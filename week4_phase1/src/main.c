// main.c, 159
// Phase 1, Time Sharing and Context Switches
// Team Name: GidOS (Members: Eloisa Esparza and Luis Avitia)

#include "spede.h"      // given SPEDE stuff
#include "handlers.h"   // handler code
#include "tools.h"      // small functions for handlers
#include "proc.h"       // processes such as Init()
#include "types.h"      // data types
#include "events.h"     // events for kernel to serve

// kernel's own data:
int current_pid;        // current selected PID; if 0, none selected
q_t ready_q, free_q;    // processes ready to run and not used
pcb_t pcb[PROC_NUM];    // process control blocks
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks
struct i386_gate *IDT_p;

void IDTEntrySet(int event_num, func_ptr_t event_addr){
  struct i386_gate *IDT_table = &IDT_p[event_num];
  fill_gate(IDT_table, (int)event_addr, get_cs(), ACC_INTR_GATE, 0);
} // end IDTEntrySet()

void Scheduler(){                 // choose a PID as current_pid to load/run
  if(current_pid != 0) return;    // if continue below, find one for current_pid

  if(ready_q.size == 0){
    cons_printf("Kernel Panic: no process to run!\n"); // big problem!
    breakpoint();                 // exec breakpoint() to go into GDB;
  }
  current_pid = DeQ(&ready_q);    // get next ready-to-run process as current_pid
  pcb[current_pid].state = RUN;   // update its state
  pcb[current_pid].cpu_time = 0;  // reset cpu_time count
}

// OS bootstrap from main() which is process 0, so we do not use this PID
int main() {
  int i;

  MyBzero((char *)&ready_q, sizeof(q_t));
  MyBzero((char *)&free_q, sizeof(q_t));

  for(i=1; i<Q_SIZE; i++){        // queue free queue with PID 1~19
    EnQ(i, &free_q);
  }
  IDT_p = get_idt_base();         // init IDT_p (locate IDT location)
  cons_printf("IDT located at DRAM addr %x (%d).\n", IDT_p, IDT_p); // show location on Target PC

  IDTEntrySet(TIMER_EVENT, TimerEvent); // set IDT entry 32 like our timer lab
  outportb(0x21, ~0x19);          // set PIC mask to open up for timer IRQ0 only

  NewProcHandler(Init);           // call NewProcHandler(Init) to create Init proc
  Scheduler();                    // call Scheduler() to select current_pid (will be 1)
  Loader(pcb[current_pid].TF_p);  // call Loader with the TF address of current_pid

  return 0;                       // compiler needs for syntax altho this statement is never exec
}

void Kernel(TF_t *TF_p) {         // kernel code exec (at least 100 times/second)
  char key;

  pcb[current_pid].TF_p = TF_p;   // save TF_P into the PCB of current_pid
  // switch according to the event_num in the TF TF_p points to {
  switch(TF_p->event_num){
    case TIMER_EVENT:
      TimerHandler();
      break;
    default: 
      cons_printf("Kernel Panic: unknown event_num %d!\n");
      breakpoint();
  }

  if(cons_kbhit()){ // if a key is pressed on Target PC
    key = cons_getchar();  // get the key
      
    switch(key){    // switch by the key obtained {
      case 'n':
        NewProcHandler(UserProc);
        break;
      case 'b':
        breakpoint();
        break;
      case 'q':
        exit(0);    // quit program
    }
  }
  Scheduler();                    // call Scheduler() to select current_pid (if needed)
  Loader(pcb[current_pid].TF_p);  // call Loader with the TF address of current_pid
} // end Kernel()
