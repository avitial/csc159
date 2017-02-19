// proc.c, 159
// all processes are coded here
// processes do not use kernel space (data.h) or code (handlers, tools, etc.)
// all must be done thru system service calls

#include "spede.h"      // cons_xxx below needs
#include "data.h"       // current_pid needed below
#include "proc.h"       // prototypes of processes
//#include "service.h"
#define LOOP 1666000 // loop to time .6 us
// Init PID 1, always ready to run, never preempted
void Init(void) {
   int i;

   while(1){
      //show on Target PC: "1.." (since Init has PID 1 as we know)
	cons_printf("1..");       
	//loop for LOOP times { // to cause approx 1 second of delay
	for(i = 0;i<LOOP; i++){
         //call asm("inb $0x80") which delay .6 microsecond
	asm("inb $0x80");
      }
   }
}

// PID 2, 3, 4, etc. mimicking a usual user process
void UserProc(void) {
   int i, my_pid;

   while(1){
      //show on Target PC: "%d..", current_pid (will change to GetPID call later)
	cons_printf("%d..", current_pid);
      //loop for LOOP times { // to cause approx 1 second of delay
	for(i = 0; i < LOOP; i++){
	//call asm("inb $0x80") which delay .6 microsecond
	asm("inb $0x80");
      }
   }
}
