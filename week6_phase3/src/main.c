// main.c, 159
// phase 2
// Team Name: GidOS (Members: Esparza, Eloisa and AvitiaMontelongo, Luis)

#include "spede.h" // given SPEDE stuff
#include "types.h" // data types
#include "handlers.h" // handler code
#include "tools.h" // small functions for handlers
#include "proc.h" // processes such as Init()
#include "events.h" // events for kernel to serve

// kernel's own data:
int current_pid, current_time, vehicle_sid; // current selected PID; if 0, none selected
q_t ready_q, free_q, sleep_q, sem_q; // processes ready to run and not used
pcb_t pcb[PROC_NUM]; // process control blocks
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks
struct i386_gate *IDT_p;
unsigned short *ch_p = 0xb8000;
sem_t sem[Q_SIZE];

void IDTEntrySet(int event_num, func_ptr_t event_addr){
	struct i386_gate *IDT_tbl = &IDT_p[event_num];
	fill_gate(IDT_tbl, (int)event_addr, get_cs(), ACC_INTR_GATE, 0);
} // end IDTEntrySet()

void Scheduler(){ // choose a PID as current_pid to load/run
//	q_t *p;

	if(current_pid != 0) return; // if continue below, find one for current_pid
//  if(current_pid == 0)pcb[0].state = READY;   
 // if(current_pid ==0) pcb[0].state = READY;


//	p = &ready_q; // &free_q
//	changed p to ready_q
	if (ready_q.size == 0){ // if ready_q.size is 0 {
		cons_printf("Kernel Panic: no process to run!\n"); // big problem!
	//	current_pid = 0;
    //addition
    breakpoint();
	}
	current_pid = DeQ(&ready_q); // get next ready-to-run process as current_pid
	pcb[current_pid].state = RUN; // update proc state
//phase3
  ch_p[current_pid*80+43] = 0xf00 + 'R';
} // end Scheduler()

// OS bootstrap from main() which is process 0, so we do not use this PID
int main() {
	int i;
//q_t *p;
// init free_q
//	p = &free_q;
//	p->size = 0;
//	p->head = 0;
//	p->tail = 0;

// init ready_q
//	p = &ready_q; 
//	p->size = 0; 
//	p->head = 0;
//	p->tail = 0;

  MyBzero((char *)&free_q, PROC_NUM);
  MyBzero((char *)&ready_q,PROC_NUM);
  MyBzero((char *)&sleep_q,PROC_NUM);
	
	current_pid = 0; // no process running
	current_time = 0; // init current time 
  vehicle_sid = 0; 

  for(i=1; i<Q_SIZE; i++){
    EnQ(i, &free_q);
  }
	// init sleep_q 
//	p = &sleep_q;
//	p->size = 0;
//	p->head = 0;
//	p->tail = 0;
  
  // init sem_q
  //p = &sem_q;
  //p->size = 0;
  //p->head = 0;
  //p->tail = 0;

  // init sem array
  for(i=1; i<PROC_NUM; i++){
    sem[i].owner = 0; 
    sem[i].passes = 0;
    sem[i].wait_q.size = 0;
//    sem[i].wait_q.head = 0;
//    sem[i].wait_q.tai = 0;
  }

	IDT_p = get_idt_base(); // init IDT_p (locate IDT location)
	cons_printf("IDT located @ DRAM addr %x (%d).\n", IDT_p, IDT_p); // show location on Target PC
	IDTEntrySet(0x20, TimerEvent);
	IDTEntrySet(0x65, SleepEvent);
	IDTEntrySet(0x64, GetPidEvent);
	IDTEntrySet(0x66, SemAllocEvent);
  IDTEntrySet(0x67, SemWaitEvent);
  IDTEntrySet(0x68, SemPostEvent);
  
  outportb(0x21, ~0x01); // set PIC mask to open up for timer IRQ0 only
  
	NewProcHandler(Init); // call NewProcHandler(Init) to create Init proc
  NewProcHandler(Vehicle);
  Scheduler(); // call scheduler to select current_pid (if needed)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
	
	return 0; // compiler needs for syntax altho this statement is never exec
} // end main()

void Kernel(TF_t *TF_p) { // kernel code exec (at least 100 times/second)
	pcb[current_pid].TF_p = TF_p; // save TF_P into the PCB of current_pid
  // switch according to the event_num in the TF TF_p points to {
	switch (TF_p->event_num){
	  case TIMER_EVENT: // if it's timer event
      TimerHandler(); // call timer event handler
	    break;
    case SLEEP_EVENT:
      SleepHandler(TF_p->eax);
      break;
    case GETPID_EVENT:
      TF_p->eax = current_pid;
      break;
    case SEMALLOC_EVENT:
      TF_p->eax = current_pid; 
      SemAllocHandler(TF_p->ebx);
      break;
    case SEMWAIT_EVENT:
      SemWaitHandler(TF_p->eax);
      break;
    case SEMPOST_EVENT:
      SemPostHandler(TF_p->eax);
  }

	if(cons_kbhit()){ // if a key is pressed on Target PC
		char key = cons_getchar(); // get the key
		switch(key){ // switch by the key obtained {
			case 'n':
			  if(ready_q.size == 0){
				  cons_printf("No more available PIDs!\n");
			  } else{
				  NewProcHandler(UserProc); // call NewProcHandler to create UserProc
			  }
			  break;
			case 'b':
			  breakpoint(); // go into gdb
			  break;
			case 'v':
         NewProcHandler(Vehicle); // call Vehicle to create vehicle proc
         break;
      case 'q':
			  exit(0); // quit program
		}
	}
	Scheduler(); // call scheduler to select current_pid (if needed)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
} // end Kernel()
