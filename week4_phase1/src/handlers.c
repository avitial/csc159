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
  if(q->size == PROC_NUM){ // this may occur for testing 
  	cons_printf("Kernel Panic: no more PID left!\n");
    //`breakpoint(); // breakpoint() into GDB
	}

  pid = DeQ(&free_q); // get 'pid' from free_q
  MyBzero((char *)&pcb[pid], sizeof(pcb_t));
  MyBzero((char *)proc_stack[pid], PROC_STACK_SIZE); // use tool to clear the PCB (indexed by 'pid')
  EnQ(pid, &ready_q); // queue pid to be ready-to-run
	pcb[pid].TF_p = (TF_t *)&proc_stack[pid][PROC_STACK_SIZE]; // point TF_p to highest area in stack
	pcb[pid].TF_p--; // space for TF
  
  // then fill out the eip of the TF
	pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
	pcb[pid].TF_p->eip = (unsigned int) p;
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
  outportb(0x20, 0x60); /// Don't forget: notify PIC event-handling done 
	
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
