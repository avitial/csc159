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

  if(free_q.size == 0){        // this may occur for testing
    cons_printf("Kernel Panic: no more PID left!\n");
    breakpoint();
  }
  pid = DeQ(&free_q);          // get 'pid' from free_q
  MyBzero((char *)&pcb[pid], sizeof(pcb_t));          // clear the PCB (indexed by 'pid')
  MyBzero((char *)&proc_stack[pid], PROC_STACK_SIZE); // clear its runtime stack
  pcb[pid].state = READY; // update process state
  EnQ(pid, &ready_q);     // queue 'pid' to be ready-to-run
  pcb[pid].TF_p = (TF_t *)&proc_stack[pid][PROC_STACK_SIZE - sizeof(TF_t)];  // point TF_p to highest area in stack (but has a space for a TF)

  // then fill out the eip of the TF
  pcb[pid].TF_p->eip = (int)p;  // new process code
  pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
  pcb[pid].TF_p->cs = get_cs();   // duplicate from current CPU
  pcb[pid].TF_p->ds = get_ds();   // duplicate from current CPU
  pcb[pid].TF_p->es = get_es();   // duplicate from current CPU
  pcb[pid].TF_p->fs = get_fs();   // duplicate from current CPU
  pcb[pid].TF_p->gs = get_gs();   // duplicate from current CPU
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void) {
  int i;

  pcb[current_pid].cpu_time++;  // upcount cpu_time of the process (PID is current_pid)
  current_time++;               // upcount the OS current time
  
  for(i=1; i<=Q_SIZE; i++){
    if((pcb[i].state == SLEEP) && (pcb[i].wake_time == current_time)){
      EnQ(i, &ready_q);         // append pid to ready q
      pcb[i].state = READY;     // update proc state
    }
  }

  if(pcb[current_pid].cpu_time == TIME_LIMIT){  // if its cpu_time reaches the preset OS time limit (see types.h)
    pcb[current_pid].state = READY; // update/downgrade its state
    EnQ(current_pid, &ready_q);     // queue its PID back to ready-to-run PID queue
    current_pid = 0;                // no running PID anymore
  }
  outportb(0x20, 0x60);             // Don't forget: notify PIC event-handling done 
  return;
}

void GetPidHandler(void){
  pcb[current_pid].TF_p->eax = (unsigned int)current_pid;
}

void SleepHandler(int sleep_amount){
  pcb[current_pid].wake_time = current_time + (100 * sleep_amount);  // calculate future wake time in pcb of current pid
  pcb[current_pid].state = SLEEP;  // update proc state
  current_pid = 0;      // reset current pid
}
