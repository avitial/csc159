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
q_t ready_q, free_q, sleep_q; // processes ready to run and not used
pcb_t pcb[PROC_NUM]; // process control blocks
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks
struct i386_gate *IDT_p;
unsigned short *ch_p = (unsigned short*)0xB8000; // init ch_p pointer to vga
sem_t sem[Q_SIZE];
port_t port[PORT_NUM];

void IDTEntrySet(int event_num, func_ptr_t event_addr){
	struct i386_gate *IDT_tbl = &IDT_p[event_num];
	fill_gate(IDT_tbl, (int)event_addr, get_cs(), ACC_INTR_GATE, 0);
} // end IDTEntrySet()

void Scheduler(){ // choose a PID as current_pid to load/run
  
  if(current_pid != 0) return; // if continue below, find one for current_pid

	if (ready_q.size == 0){ // if ready_q.size is 0 {
		cons_printf("Kernel Panic: no process to run!\n"); // big problem!
    breakpoint(); // alternative
	}
  current_pid = DeQ(&ready_q); // get next ready-to-run process as current_pid
  pcb[current_pid].state = RUN; // update proc state
  pcb[current_pid].cpu_time = 0; // reset proc cpu_time count 
  ch_p[current_pid*80+43] = 0xf00 + 'R'; // phase 3 

} // end Scheduler()

// OS bootstrap from main() which is process 0, so we do not use this PID
int main() {
	int i;
  
  MyBzero((char *)&ready_q, sizeof(q_t));
  MyBzero((char *)&free_q, sizeof(q_t));
  MyBzero((char *)&sleep_q, sizeof(q_t));
  MyBzero((char *)&sem, (sizeof(sem_t))*Q_SIZE);

	current_time = 0;       // init current time 
  vehicle_sid = -1;       // vehicle proc running

  // queue free_q with pid 1~19
  for(i=1; i<Q_SIZE; i++){
    EnQ(i, &free_q);
  }

	IDT_p = get_idt_base(); // init IDT_p (locate IDT location)
	cons_printf("IDT located @ DRAM addr %x (%d).\n", IDT_p, IDT_p); // show location on Target PC
	IDTEntrySet(0x20, TimerEvent);
	IDTEntrySet(0x64, GetPidEvent);
  IDTEntrySet(0x65, SleepEvent);
	IDTEntrySet(0x66, SemAllocEvent);
  IDTEntrySet(0x67, SemWaitEvent);
  IDTEntrySet(0x68, SemPostEvent);
  IDTEntrySet(0x69, SysPrintEvent);
  IDTEntrySet(0x23, PortEvent);
  IDTEntrySet(0x6A, PortAllocEvent);
  IDTEntrySet(0x6B, PortWriteEvent);
  IDTEntrySet(0x6C, PortReadEvent);

  outportb(0x21, ~0x01);  // set PIC mask to open up for timer IRQ0 only
  
	NewProcHandler(Init);   // call NewProcHandler(Init) to create Init proc
  Scheduler();            // call scheduler to select current_pid (if needed)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
  return 0;               // compiler needs for syntax altho this statement is never exec
} // end main()

void Kernel(TF_t *TF_p) { // kernel code exec (at least 100 times/second)
	//char key;
  
  pcb[current_pid].TF_p = TF_p; // save TF_P into the PCB of current_pid
  
  // switch according to the event_num in the TF TF_p points to {
	switch (TF_p->event_num){
	  case TIMER_EVENT:     // if it's timer event
      TimerHandler();     // call timer event handler
	    break;
    case SLEEP_EVENT:
      SleepHandler(TF_p->eax);
      break;
    case GETPID_EVENT:
      GetPidHandler();    // call getpid event handler 
      break;
    case SEMALLOC_EVENT:
      SemAllocHandler(TF_p->eax);
      break;
    case SEMWAIT_EVENT:
      SemWaitHandler(TF_p->eax);
      break;
    case SEMPOST_EVENT:
      SemPostHandler(TF_p->eax);
      break;
    case SYSPRINT_EVENT:
      SysPrintHandler((char *)TF_p->eax);
      break;
    case PORT_EVENT:
      PortHandler();
      break;
    case PORTALLOC_EVENT:
      PortAllocHandler(&TF_p->eax);
      break;
    case PORTWRITE_EVENT:
      PortWriteHandler((char)TF_p->eax, TF_p->ebx);
      break;
    case PORTREAD_EVENT:
      PortReadHandler((char *)TF_p->eax, TF_p->ebx);
      break;
    default:
      cons_printf("Kernel Panic: unknown event_num %d!\n"); 
      breakpoint();
  }

  // moving the keyboard input to Init (in proc.c/h)
	/*if(cons_kbhit()){ // if a key is pressed on Target PC
		key = cons_getchar(); // get the key

		switch(key){ // switch by the key obtained {
			case 'n':
				NewProcHandler(UserProc); // call NewProcHandler to create UserProc
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
  */
	Scheduler();          // call scheduler to select current_pid (if needed)
	Loader(pcb[current_pid].TF_p); // call Loader with the TF address of current_pid
} // end Kernel()
