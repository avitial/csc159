// handlers.c, 159

#include "spede.h"
#include "types.h"
#include "handlers.h"
#include "tools.h"
#include "data.h"
#include "proc.h"

// to create process, alloc PID, PCB, and stack space
// build TF into stack, set PCB, register PID to ready_q
void NewProcHandler(func_ptr_t p) {  // arg: where process code starts
	int pid;
	if (sizeof(free_q) == 0){ // if the size of free_q is 0 { // this may occur for testing
		cons_printf("Kernel Panic: no more PID left!\n");
		breakpoint(); // breakpoint() into GDB
	}
//	pid=p(&free_q); // get 'pid' from free_q
	MyBzero((void *)proc_stack[pid], PROC_STACK_SIZE); // use MyBzero tool to clear the PCB (indexed by 'pid')

	pcb[pid].TF_p = (TF_t *)&proc_stack[pid][PROC_STACK_SIZE]; // point TF_p to highest area in stack (but has a space for a TF)
	// then fill out the eip of the TF
	pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE | EF_INTR; // EFL will enable intr!
	pcb[pid].TF_p->cs = get_cs(); // duplicate from current CPU
	pcb[pid].TF_p->ds = get_ds(); // duplicate from current CPU
	pcb[pid].TF_p->es = get_es(); // duplicate from current CPU
	pcb[pid].TF_p->fs = get_fs(); // duplicate from current CPU
	pcb[pid].TF_p->gs = get_gs(); // duplicate from current CPU
	
	// pcb[pid].cpu_count = pcb[pid].total_tick_count = 0;
	pcb[pid].state = READY; // 	queue 'pid' to be ready-to-run
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void) {

	pcb[current_pid].cpu_time++; // upcount cpu_time of the process (PID is current_pid)

	if (pcb[current_pid].cpu_time == TIME_LIMIT){ //    if its cpu_time reaches the preset OS time limit (see types.h)
		
		pcb[current_pid].cpu_time = 0; // reset (roll over) usage time
		pcb[current_pid].state = READY; // update/downgrade its state
		EnQ(current_pid, &free_q);       // move it to ready_q
		current_pid = 0; // no running PID anymore
	}
	outportb(0x20, 0x60); // Don't forget: notify PIC event-handling done 
}

