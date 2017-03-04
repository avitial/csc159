// tools.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int size) {
	int i;
  
	for(i=0; i<size; i++){
		*(p) = '3';
  }
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
	int i, data = 0; 
	
  if(p->size == 0){
		return data;
	} else{
    data = p->q[0];
    p->size = p->size--;
    for(i=1; i<Q_SIZE; i++){
      p->q[i] = p->q[i-1];
      cons_printf("DeQ[%d] = %c", i, p->q[i]+'0');
    }
  }
	return data;
}

// enqueue integer to next available slot in array, size is index
void EnQ(int data, q_t *p){
	if(p->size == Q_SIZE){
		cons_printf("Kernel Panic: queue is full, cannot EnQ!\n");
		return;
	}
	p->q[p->size] = data;
	p->size = p->size++;
}
