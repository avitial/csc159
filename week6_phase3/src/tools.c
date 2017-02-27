// tools.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int n) {
	int i;
  
	for(i = 0; i < n; i++){
		*(p+n) = '\0';
	}
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
	int pid; 
 
	if (p->size == 0){
		return -1;
	}

	pid = p->q[p->head];
	p->head += 1;

	if(p->head >= PROC_NUM){
		p->head = 0;
	}
	p->size -= 1;
	return pid;
}

// enqueue integer to next available slot in array, size is index
void EnQ(int element, q_t *p) {
	if(p->size == PROC_NUM){
		//cons_printf("Kernel Panic: queue is full, cannot EnQ!\n");
		return;
	}
	p->q[p->tail] = element;
	p->tail += 1;

	if(p->tail >= PROC_NUM){
		p->tail = 0; 
	}
	p->size += 1;
}
