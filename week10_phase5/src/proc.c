// proc.c, 159
// all processes are coded here
// processes do not use kernel space (data.h) or code (handlers, tools, etc.)
// all must be done thru system service calls

#include "services.h"
#include "spede.h"      // cons_xxx below needs
#include "data.h"       // current_pid needed below
#include "proc.h"       // prototypes of processes
#include "handlers.h"

// Init PID 1, always ready to run, never preempted
void Init(void) {
  int i;
  char key;
  char str[] = " Hello, World! Team GidOS: Eloisa Esparza and Luis Avitia\n\r";

  while(1){
    if(cons_kbhit()){           // if a key is pressed on Target PC
      key = cons_getchar();     // get the key

      switch(key){              // switch by the key obtained {
        case 'p':
          SysPrintHandler(str); // call SysPrintHandler to print
          break;
        case 'b':
          breakpoint();         // go into gdb
          break;
        case 'q':
          exit(0);              // quit program
      }   
    }

	  for(i=0; i<FAST_LOOP; i++){ //loop for LOOP times { // to cause approx 1 second of delay
      asm("inb $0x80");         // call asm("inb $0x80") which delay .6 microsecond
    }
  }
}

// PID 2, 3, 4, etc. mimicking a usual user process
void UserProc(void) {
  int pid, sleep_amount;
  pid = GetPid();
  
  while(1){
    sleep_amount = pid; 
	  cons_printf("%d..", pid);   //show on Target PC: "%d..", current_pid 
    Sleep(sleep_amount);
  }
}

void Vehicle(void){             //phase 3 tester (multiple processes)
  int i, pid;
  int sleep_amount = 1;
  if(vehicle_sid == -1){
    vehicle_sid = SemAlloc(3); //max passes 3
  }
  pid = GetPid();
  while(1){
    ch_p [pid*80+45]= 0xf00 + 'f';  //show i'm off the bridge
    
    for(i =0; i<FAST_LOOP; i++){    //spend a sec in RUN state
      asm("inb $0x80");           
    }
    SemWait(vehicle_sid);           //ask for a pass
    ch_p[pid*80+45] = 0xf00 + 'o';  //show i'm on the bridge
    Sleep(sleep_amount);
    SemPost(vehicle_sid);           //return the pass
  }
}
