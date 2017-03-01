// handlers.c, 159

#include "spede.h"
#include "types.h"
#include "handlers.h"
#include "tools.h"
#include "data.h"
#include "proc.h"

// to create process, alloc PID, PCB, and stack space
// build TF into stack, set PCB, register PID to ready_q
void NewProcHandler(func_ptr_t p) {  // arg: where process code starts
	int pid; 
  q_t *q;
  q = &free_q;
  if((q->size == PROC_NUM)){ // this may occur for testing 
  	cons_printf("Kernel Panic: no more PID left!\n");
    //`breakpoint(); // breakpoint() into GDB
	}

  pid = DeQ(&free_q); // get 'pid' from free_q
  MyBzero((char *)&pcb[pid], sizeof(pcb_t));
  MyBzero((char *)proc_stack[pid], PROC_STACK_SIZE); // use tool to clear the PCB (indexed by 'pid')
	
  pcb[pid].TF_p = (TF_t *)&proc_stack[pid][PROC_STACK_SIZE]; // point TF_p to highest area in stack
	pcb[pid].TF_p--; // space for TF
  
  // then fill out the eip of the TF
	pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
	pcb[pid].TF_p->eip = (unsigned int) p; // new process code
	pcb[pid].TF_p->cs = get_cs(); // duplicate from current CPU
	pcb[pid].TF_p->ds = get_ds(); // duplicate from current CPU
	pcb[pid].TF_p->es = get_es(); // duplicate from current CPU
	pcb[pid].TF_p->fs = get_fs(); // duplicate from current CPU
	pcb[pid].TF_p->gs = get_gs(); // duplicate from current CPU

  pcb[pid].cpu_time = pcb[pid].total_cpu_time = 0;
	pcb[pid].state = READY; // 	queue 'pid' to be ready-to-run
  if(pid != 0) EnQ(pid, &ready_q); // pid1 not queued
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void) {
  q_t *p;
  outportb(0x20, 0x60); /// Don't forget: notify PIC event-handling done 
  current_time++;
  p = &sleep_q;

  while(!(p->size == 0) && (pcb[sleep_q.q[sleep_q.head]].wake_time <= current_time)){
    int pid_temp = DeQ(&sleep_q);
    pcb[pid_temp].state = READY;
    EnQ(pid_temp, &ready_q);
  }
  if(current_pid == 0) return; // if pid0, no need to handle it
  
  pcb[current_pid].cpu_time++; // upcount cpu_time of the process (PID is current_pid)

  if (pcb[current_pid].cpu_time == TIME_LIMIT){ // if its cpu_time reaches the preset OS time limit
		pcb[current_pid].cpu_time = 0; // reset (roll over) usage time
		pcb[current_pid].total_cpu_time += TIME_LIMIT; // total time sumation
    pcb[current_pid].state = READY; // update/downgrade its state
		EnQ(current_pid, &ready_q); // move it to ready_q
	  current_pid = -1; // no longer runs
  }
}

void SleepHandler(int sleep_amount){
  q_t *p, q_temp; 

  p = &q_temp;
  p->size = 0;
  p->head = 0;
  p->tail = 0;
  
  p = &sleep_q;

  pcb[current_pid].wake_time = (current_time + sleep_amount * 100);
  while(!(p->size == 0) && (pcb[sleep_q.q[sleep_q.head]].wake_time <= pcb[current_pid].wake_time)){
    int pid_temp = DeQ(&sleep_q);
    EnQ(pid_temp, &q_temp);
  }
  EnQ(current_pid, &q_temp);
  p = &sleep_q;
  while(!(p->size == 0)){
    EnQ(DeQ(&sleep_q), &q_temp);
  }
  p = &q_temp;
  while(!(p->size == 0)){
    EnQ(DeQ(&q_temp), &sleep_q);
  }
  pcb[current_pid].state = SLEEP;
  current_pid = -1; 
}

void SemAllocHandler(int passes){
  int sid;
  q_t *p, *p_temp;
  p = &sem_q;
  if(!(p->size == 0)){
      sid = DeQ(&sem_q);
      sem[sid].passes = passes;
      p_temp = &(sem[sid].wait_q);
      p_temp->size = 0;
      p_temp->head = 0;
      p_temp->tail = 0;
  } else{
    sid = -1; 
  }
  return sid; 
}

void SemWaitHandler(int sid){
  if(sem[sid].passes > 0){
    sem[sid].passes -= 1; 
  } else{
    pcb[current_pid].state = WAIT;
    EnQ(current_pid, &(sem[sid].wait_q));
    current_pid = -1;
  }
}

void SemPostHandler(int sid){
  q_t *p;
  p = &(sem[sid].wait_q);
  if(!(p->size == 0)){
    int free_pid = DeQ(&(sem[sid].wait_q));
    pcb[free_pid].state = READY;
    EnQ(free_pid, &ready_q);
  } else{
    sem[sid].passes += 1;
  }
}
