// proc.c, 159
// all processes are coded here
// processes do not use kernel space (data.h) or code (handlers, tools, etc.)
// all must be done thru system service calls

#include "spede.h"      // cons_xxx below needs
#include "data.h"       // current_pid needed below
#include "proc.h"       // prototypes of processes
#include "services.h"

// Init PID 1, always ready to run, never preempted
void Init(void) {
  int i;

  while(1){  // forever loop
    cons_printf("1..");     // show on Target PC: "1.." (since Init has PID 1 as we know)

    for(i=0; i<FAST_LOOP; i++){  // cause approx 1 second of delay
      asm("inb $0x80");     // delay .6 microsecond
    }
  }
}

// PID 2, 3, 4, etc. mimicking a usual user process
void UserProc(void) {
  int pid, sleep_amount;
  
  pid = GetPid(); 
  
  while(1){  // forever loop {
    sleep_amount = pid;
    cons_printf("%d..", pid); // show current_pid (will change to GetPID call later)
    Sleep(sleep_amount);  
  }
}
