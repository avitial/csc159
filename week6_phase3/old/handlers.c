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
  MyBzero((char *)&proc_stack[pid], PROC_STACK_SIZE); // use tool to clear the PCB (indexed by 'pid')

  pcb[pid].TF_p = (TF_t *)&proc_stack[pid][PROC_STACK_SIZE - sizeof(TF_t)]; // point TF_p to highest area in stack
  
  // then fill out the eip of the TF
	pcb[pid].TF_p->eip = (int) p; // new process code
	pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
  pcb[pid].TF_p->cs = get_cs(); // duplicate from current CPU
	pcb[pid].TF_p->ds = get_ds(); // duplicate from current CPU
	pcb[pid].TF_p->es = get_es(); // duplicate from current CPU
	pcb[pid].TF_p->fs = get_fs(); // duplicate from current CPU
	pcb[pid].TF_p->gs = get_gs(); // duplicate from current CPU

  pcb[pid].cpu_time = 0;        //pcb[pid].total_cpu_time = 0;
  pcb[pid].state = READY;
  EnQ(pid, &ready_q);

  if(pid>9){
    ch_p[pid*80+40]=0xf00+ (pid/10+'0');
    ch_p[pid*80+41]=0xf00+(pid%10+'0');
  } else{
    ch_p[pid*80+40]=0xf00+pid+'0';
  }
  ch_p[pid*80+43] = 0xf00 +'r';
}

void GetPidHandler(void){
  pcb[current_pid].TF_p->eax = current_pid;
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void){
  int i;
  current_time++;              // phase 1
  pcb[current_pid].cpu_time++; // upcount cpu_time of the process (PID is current_pid)
  
  if(pcb[current_pid].cpu_time == TIME_LIMIT){ // if its cpu_time reaches the preset OS time limit
    pcb[current_pid].state = READY; // update/downgrade its state
    EnQ(current_pid, &ready_q);     // move it to ready_q
    ch_p[current_pid*80+43] = 0xf00 +'r';
    current_pid = 0; // no longer runs
  } 
  
  for(i=0; i<Q_SIZE; i++){    // phase 2  
    if((pcb[i].state == SLEEP) && (pcb[i].wake_time == current_time)){ 
      DeQ(&sleep_q);
      EnQ(i, &ready_q);     // append pid to ready_q
      pcb[i].state = READY; // update proc state
      ch_p[i*80+43] = 0xf00 +'r';
    }
  }
  outportb(0x20, 0x60); /// Don't forget: notify PIC event-handling done
}

void SleepHandler(int sleep_amount){
  pcb[current_pid].wake_time = (current_time + 100 * sleep_amount); // calc future wake time in pcb
  EnQ(current_pid, &sleep_q);
  pcb[current_pid].state = SLEEP; // update proc state
  ch_p[current_pid*80+43] = 0xf00 +'S';
  current_pid = 0; // reset current_pid
}

void SemAllocHandler(int passes){
  int sid;
  for(sid=0; sid<PROC_NUM; sid++){
    if(sem[sid].owner == 0) break; 
  }
  if(sid == PROC_NUM){
    cons_printf("Kernel panic: no more semaphores left!\n");
    return; 
  }
  sem[sid].owner = current_pid;
  sem[sid].passes = passes;
  MyBzero((char *)&sem[sid].wait_q, Q_SIZE);
  sem[sid].wait_q.size = 0; 
  pcb[current_pid].TF_p -> ebx = sid; 
  
}

void SemWaitHandler(int sid){
  if(sem[sid].passes > 0){
    sem[sid].passes--;
    ch_p[48] = 0xf00 + sem[sid].passes + '0';
    //cons_printf("SEMWAIT: passes = %d\t", sem[sid].passes);
    return; 
  } else{ 
    EnQ(current_pid, &sem[sid].wait_q);
    pcb[current_pid].state = WAIT;
    ch_p[current_pid * 80 + 43] = 0xf00 + 'W';
    current_pid = 0;
  }
}

void SemPostHandler(int sid){
  int free_pid = 0;
  if((sem[sid].wait_q.q[0] != 0)){
    free_pid = DeQ(&sem[sid].wait_q);
    EnQ(free_pid, &ready_q);
    pcb[free_pid].state = READY;
    ch_p[free_pid*80+43] = 0xf00 +'r';
  } else{
    sem[sid].passes++;// = sem[sid].passes +1;
    ch_p[48] = 0xf00 + sem[sid].passes + '0';
  //  cons_printf("SEMPOST: passes = %d\t", sem[sid].passes);
  }
}
