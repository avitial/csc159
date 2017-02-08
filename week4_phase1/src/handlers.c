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

   if the size of free_q is 0 { // this may occur for testing
      show on Target PC: "Kernel Panic: no more PID left!\n"
      return;                   // alternative: breakpoint() into GDB
   }

   get 'pid' from free_q
   use MyBzero tool to clear the PCB (indexed by 'pid')
   also, clear its runtime stack
   update process state
   queue 'pid' to be ready-to-run

   point TF_p to highest area in stack (but has a space for a TF)
   then fill out the eip of the TF
   the eflags in the TF becomes: EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
   the cs in the TF is get_cs();   // duplicate from current CPU
   the ds in the TF is get_ds();   // duplicate from current CPU
   the es in the TF is get_es();   // duplicate from current CPU
   the fs in the TF is get_fs();   // duplicate from current CPU
   the gs in the TF is get_gs();   // duplicate from current CPU
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void) {
   upcount cpu_time of the process (PID is current_pid)

   if its cpu_time reaches the preset OS time limit (see types.h)
      update/downgrade its state
      queue its PID back to ready-to-run PID queue
      reset current_pid (to 0)  // no running PID anymore
   }

   Don't forget: notify PIC event-handling done 
}

