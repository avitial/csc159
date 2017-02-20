// main.c, 159
// this is kernel code for phase 1
// Team Name: GidOS (Members: Esparza, Eloisa and AvitiaMontelongo, Luis)

#include "spede.h" // given SPEDE stuff
#include "handlers.h" // handler code
#include "tools.h" // small functions for handlers
#include "proc.h" // processes such as Init()
#include "types.h" // data types
#include "events.h" // events for kernel to serve

// kernel's own data:
int current_pid; // current selected PID; if 0, none selected
q_t ready_q, free_q; // processes ready to run and not used
pcb_t pcb[PROC_NUM]; // process control blocks
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks

void Scheduler() { // choose a PID as current_pid to load/run
	if(current_pid > 0) return; // if continue below, find one for current_pid

//	if (current_pid == 0) pcb[0].state = READY;   

	if ((ready_q.size)==0){ // if ready_q.size is 0 {
		current_pid = 0; // no process, throw kernel panic msg
		cons_printf("Kernel Panic: no process to run!\n"); // big problem!
		breakpoint(); // go into GDB
	}
	else{
		current_pid = DeQ(&ready_q); // get next ready-to-run process as current_pid
		pcb[current_pid].state = RUN; // update proc state
		pcb[current_pid].cpu_time = 0; // reset cpu_time count
	}
}

// OS bootstrap from main() which is process 0, so we do not use this PID
int main() {
	int i;
	q_t *p;
  struct i386_gate *IDT_p; // DRAM location where IDT is
  
  p = (&free_q);
  p->size = 0; 

  p = (&ready_q);
  p->size = 0;

  MyBzero((void *)proc_stack[], PROC_STACK_SIZE); // use tool function MyBzero to clear the two PID queues
	IDT_p = get_idt_base(); // init IDT_p (locate IDT location)
	cons_printf("IDT located @ DRAM addr %x (%d).\n", IDT_p, IDT_p); // show location on Target PC
	SetIDTEntry(32, TimerEntry); // set IDT entry 32 like our timer lab
	outportb(0x21, ~0x01); // set PIC mask to open up for timer IRQ0 only

	for (i = 1; i<NUM_PROC; i++){ //queue free queue with PID 1~19
		pcb[i].state = FREE;
		EnQ(i, &free_q);
	}
	current_pid=1; 
	NewProcHandler(Init); // call NewProcHandler(Init) to create Init proc
	Scheduler(); // call Scheduler() to select current_pid(will be 1)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
	return 0; // compiler needs for syntax altho this statement is never exec
}

void Kernel(TF_t *TF_p) { // kernel code exec (at least 100 times/second)
	char key;

	pcb[current_pid].TF_p = TF_p; // save TF_P into the PCB of current_pid

	// switch according to the event_num in the TF TF_p points to {
	switch (TF_p->event_num){
		case TIMER_EVENT: // if it's timer event
    		TimerHandler(); // call timer event handler
			break;
		default: 
			cons_printf("Kernel Panic: unknown event_num %d!\n");
			breakpoint(); // go into GDB 
			break;
	}
	if(cons_kbhit()){ // if a key is pressed on Target PC
		char key = cons_getchar(); // get the key

		switch(key){ // switch by the key obtained {
			case 'n': // if it's 'n'
				NewProcHandler(DeQ(&free_q)); // call NewProcHandler to create UserProc
			case 'b': // if it's 'b'
				breakpoint(); break; // go into GDB
		}
	}
	Scheduler(); // call scheduler to select current_pid (if needed)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
}

