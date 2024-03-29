// tools.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int size) {
  while(size--) *p++=0; 
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
	int i, data = 0; 
	
  if(p->size == 0){
		return data;
	} else{
    data = p->q[0];
    
    for(i=0; i<Q_SIZE; i++){
      p->q[i] = p->q[i+1];  
    }
    p->size--;
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
	p->size++;
}
