//************************************************************
// NAME: Avitia, Luis E
// Phase 0, Exercise 4 -- Timer Event
// main.c
//************************************************************
#define LOOP 1666000  // handy LOOP to time .6 microseconds
#include "spede.h"
#include "events.h"  // needs addr of TimerEvent

typedef void (* func_ptr_t)();

struct i386_gate *IDT_p; 

void RunningProcess(void){
  int i;
  for(i=0; i<LOOP; i++) IO_DELAY();//asm("inb $0x80"); // delays 1 sec
  cons_putchar('z'); // prints char z on screen
}
int main(){
  char ch;
  IDT_p = get_idt_base();  // get IDT location
  cons_printf("IDT located @ DRAM addr %x (%d).\n", IDT_p, IDT_p);  // show: "IDT located at DRAMM addr %x (%d).\n" (both address of IDT)

  fill_gate(&IDT_p[TIMER_EVENT], (int)TimerEvent, get_cs(), ACC_INTR_GATE,0);
  outportb(0x21, ~0x01); // 0x21 is PIC mask, ~1 is mask
  asm("sti");  // set/enable intr in CPU EFLAGS reg

  //call RunningProcess here to run until a key is pressed
  while(1){
    if(cons_kbhit()){
      ch = cons_getchar();
      break;
    }else{
      RunningProcess();
    }
  }
  return 0;  // main() ends
}
