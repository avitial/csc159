// tools.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int size) {
//   loop size times during which:
//      set where p points to to 0 and increment p
int z;
for(z = 0; z < size; z++){
	p-> 0;
	p++;
}
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
   int i, data = 0;

if(p-> size == 0) return 0;
	data = p->q[0];
	p->size--;
	for(i = 0;i < size; i++){
 		p->[i] = p->q[i+1];
	}
return data;

}

// enqueue integer to next available slot in array, size is index
void EnQ(int data, q_t *p) {
	if( p-> size == Q_SIZE){
		cons_printf("Kernel Panic: queue is full, cannot EnQ!\n");
		return;
	}
	p->q[size] = data;
	p-> size++;

}

