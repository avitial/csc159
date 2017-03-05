// handlers.c, 159

#include "spede.h"
#include "types.h"
#include "handlers.h"
#include "tools.h"
#include "data.h"
#include "proc.h"

// to create process, alloc PID, PCB, and stack space
// build TF into stack, set PCB, register PID to ready_q
void NewProcHandler(func_ptr_t p){  // arg: where process code starts
	int pid; 
  if((free_q.size == 0)){ // this may occur for testing 
  	cons_printf("Kernel Panic: no more PID left!\n");
    breakpoint(); // breakpoint() into GDB
	}

  pid = DeQ(&free_q); // get 'pid' from free_q
  MyBzero((char *)&pcb[pid], sizeof(pcb_t));
  MyBzero((char *)proc_stack[pid], PROC_STACK_SIZE); // use tool to clear the PCB (indexed by 'pid')
	//pcb[pid].state = READY;
  //EnQ(pid, &ready_q);

  pcb[pid].TF_p = (TF_t *)&proc_stack[pid][4032]; // point TF_p to highest area in stack
	pcb[pid].TF_p--; // space for TF
  
  // then fill out the eip of the TF
	pcb[pid].TF_p->eip = (int) p; // new process code
	pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
  pcb[pid].TF_p->cs = get_cs(); // duplicate from current CPU
	pcb[pid].TF_p->ds = get_ds(); // duplicate from current CPU
	pcb[pid].TF_p->es = get_es(); // duplicate from current CPU
	pcb[pid].TF_p->fs = get_fs(); // duplicate from current CPU
	pcb[pid].TF_p->gs = get_gs(); // duplicate from current CPU

  pcb[pid].cpu_time = pcb[pid].total_cpu_time = 0;
  if(pid != 0){ // phase 3
    pcb[pid].state = READY;
    EnQ(pid, &ready_q); // pid1 not queued
    
    if(pid>9){
      ch_p[pid*80+40]=0xf00+ (pid/10+'0');
      ch_p[pid*80+41]=0xf00+(pid%10+'0');
    } else{
      ch_p[pid*80+40]=0xf00+pid+'0';
    }
    ch_p[pid*80+43] = 0xf00 +'r';
  }
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void){
  int i = 1, pid;
  // phase 1
  pcb[current_pid].cpu_time++; // upcount cpu_time of the process (PID is current_pid)
  if(pcb[current_pid].cpu_time == TIME_LIMIT){ // if its cpu_time reaches the preset OS time limit
    pcb[current_pid].state = READY; // update/downgrade its state
    EnQ(current_pid, &ready_q); // move it to ready_q
    current_pid = 0; // no longer runs
  } 

  // phase 2
 // pcb[current_pid].current_time++; // increase os curent_time
 // int i=1;
 // 
  for(i; i<PROC_NUM; i++){
    if((pcb[i].state == SLEEP) && (pcb[i].wake_time == current_time)){
      pid = DeQ(&sleep_q);
      EnQ(pid, &ready_q);
      pcb[i].state = READY;
    }
  }

  //while((pcb[current_pid].state == SLEEP) && (pcb[sleep_q.q[sleep_q.size]].wake_time == current_time)    ){
   // int pid = DeQ(&sleep_q); 
   // EnQ(pid, &ready_q); // append pid to readu_q
   // pcb[pid].state = READY; // update proc state
 // }

  outportb(0x20, 0x60); /// Don't forget: notify PIC event-handling done
}

void SleepHandler(int sleep_amount){
  pcb[current_pid].wake_time = (current_time + 100 * sleep_amount); // calc future wake time in pcb
  pcb[current_pid].state = SLEEP; // update proc state
  current_pid = 0; // reset current_pid
}

void SemAllocHandler(int passes){
  int sid;
  //if(sem_q.size == 0){
      sid = DeQ(&sem[0].wait_q);
      sem[sid].passes = passes;
      sem[sid].wait_q.size = 0;
      sem[sid].owner = sid;
  //} else
  //  sid = 0; 
  //}
}

void SemWaitHandler(int sid){
  if(sem[sid].passes > 0){
    sem[sid].passes = sem[sid].passes - 1; 
  } else{
    pcb[current_pid].state = WAIT;
    EnQ(current_pid, &(sem[sid].wait_q));
    current_pid = 0;
  }
}

void SemPostHandler(int sid){
  if(!(sem[sid].wait_q.size == 0)){
    int free_pid = DeQ(&(sem[sid].wait_q));
    EnQ(free_pid, &ready_q);
    pcb[free_pid].state = READY;
  } else{
    sem[sid].passes = sem[sid].passes + 1;
  }
}
