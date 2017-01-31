//************************************************************
// NAME: Avitia, Luis E
// Phase 0, Exercise 4 -- Timer Evenr
// main.c
//************************************************************
#define LOOP 1666000  // handy LOOP to time .6 microseconds
#include "spede.h"
#include "events.h"  // needs addr of TimerEvent

typedef void (* func_ptr_t){();
struct i386_gate *IDT_p; 

void RunningProcess(void){
   int i;

   forever loop:
     
      loop LOOP times doing: asm(""); to delay CPU for about 1 second
      // show 'z' per second
}

int main(){
   IDT_p = get_idt_base();  // get IDT location
   show: "IDT located at DRAMM addr %x (%d).\n" (both address of IDT)

   fill_gate(&IDT_p[TIMER_EVENT], (int)TimerEvent, get_cs(), ACC_INTR_GATE,0);
   outportb(0x21, ~1); // 0x21 is PIC mask, ~1 is mask
   asm("sti");  // set/enable intr in CPU EFLAGS reg

   call RunningProcess here to run until a key is pressed
   return 0;  // main() ends
}
