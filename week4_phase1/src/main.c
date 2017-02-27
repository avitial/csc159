// main.c, 159
// this is kernel code for phase 1
// Team Name: GidOS (Members: Esparza, Eloisa and AvitiaMontelongo, Luis)

#include "spede.h" // given SPEDE stuff
#include "types.h" // data types
#include "handlers.h" // handler code
#include "tools.h" // small functions for handlers
#include "proc.h" // processes such as Init()
//#include "types.h" // data types
#include "events.h" // events for kernel to serve

// kernel's own data:
int current_pid; // current selected PID; if 0, none selected
q_t ready_q, free_q; // processes ready to run and not used
pcb_t pcb[PROC_NUM]; // process control blocks
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks

void Scheduler(){ // choose a PID as current_pid to load/run
  q_t *p;

  if(current_pid > 0) return; // if continue below, find one for current_pid
	if(current_pid == 0)pcb[0].state = READY;   

  p = &ready_q;
	if (p->size == 0){ // if ready_q.size is 0 {
	  cons_printf("Kernel Panic: no process to run!\n"); // big problem!
    current_pid = 0; 
	} else{
      current_pid = DeQ(&ready_q); // get next ready-to-run process as current_pid
	}
  pcb[current_pid].state = RUN; // update proc state
}

// OS bootstrap from main() which is process 0, so we do not use this PID
int main() {
	int i;
  q_t *p;
  struct i386_gate *IDT_p; // DRAM location where IDT is

  p=&free_q;
  p->size = 0;
  p->head = 0;
  p->tail = 0;
  

  p = &ready_q; 
  p->size = 0; 
  p->head = 0;
  p->tail = 0;

for(i=1; i<PROC_NUM; i++){
    pcb[i].state = FREE;
    EnQ(i, &free_q);
  } 
current_pid = -1;

  IDT_p = get_idt_base(); // init IDT_p (locate IDT location)
  cons_printf("IDT located @ DRAM addr %x (%d).\n", IDT_p, IDT_p); // show location on Target PC
  fill_gate(&IDT_p[TIMER_EVENT], (int)TimerEvent, get_cs(), ACC_INTR_GATE, 0);
  outportb(0x21, ~0x01); // set PIC mask to open up for timer IRQ0 only

  
  NewProcHandler(Init); // call NewProcHandler(Init) to create Init proc
  //current_pid = 0; 
  Scheduler(); // call Scheduler() to select current_pid(will be 1)
  //EI();
  Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
	return 0; // compiler needs for syntax altho this statement is never exec
}

void Kernel(TF_t *TF_p) { // kernel code exec (at least 100 times/second)
	pcb[current_pid].TF_p = TF_p; // save TF_P into the PCB of current_pid
  // switch according to the event_num in the TF TF_p points to {
	switch (TF_p->event_num){
	  case TIMER_EVENT: // if it's timer event
      TimerHandler(); // call timer event handler
	    break;
  }

	if(cons_kbhit()){ // if a key is pressed on Target PC
	   char key = cons_getchar(); // get the key
		
		q_t *p;
		p = &free_q;

		switch(key){ // switch by the key obtained {
		  case 'n': // if it's 'n'
	if(p->size == 0){
	cons_printf("No more PIDs left");
	}else{
        NewProcHandler(UserProc); // call NewProcHandler to create UserProc
	}        
	break;
      case 'b': // if it's 'b'
			  breakpoint(); // go into gdb
        break;
		  case 'q':
        exit(0); // quit program
    }
	}
	Scheduler(); // call scheduler to select current_pid (if needed)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
}
