/* Name: Avitia, Luis E
 * main.c, Phase 0, Exercise 4 -- Timer Event
 */

#define LOOP 1666000 // loop to time .6 us
#include "spede.h"
#include "events.h" // needs addr of TimerEvent

typedef void (* func_ptr_t)();
struct i386_gate *IDT_p; 

void RunningProcess(void){
	int i;
	for(i=0; i<LOOP; i++) asm("inb $0x80"); // delays 1 sec
	cons_putchar('z'); // prints char z on screen
}

int main(){
	IDT_p = get_idt_base(); // get IDT location
	cons_printf("IDT located @ DRAM addr %x (%d).\n", IDT_p, IDT_p); // show IDT addr

	fill_gate(&IDT_p[TIMER_EVENT], (int)TimerEvent, get_cs(), ACC_INTR_GATE,0);
	outportb(0x21, ~0x01); // 0x21 is PIC mask, ~1 is mask
	asm("sti"); // set/enable intr in CPU EFLAGS reg

	while(1){
		if(cons_kbhit()){
			break; // kb pressed, exit loop
		}else{
			RunningProcess(); // run process until kb is pressed
		}
	}
	return 0; // main() ends
}
