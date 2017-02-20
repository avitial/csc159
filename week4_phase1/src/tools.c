// tools.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int size) {
  int z;
  for(z = 0; z < size; z++){
    *(p+size) = '0';
  }
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
  int i, data = 0;
  i = p->q[p->size];
  p->size+=1;

  if(p->size >= PROC_NUM){
    p->size = 0;
  }
  
  p->size -= 1;
  return data;
}

// enqueue integer to next available slot in array, size is index
void EnQ(int data, q_t *p) {
	if( p-> size == Q_SIZE){
		cons_printf("Kernel Panic: queue is full, cannot EnQ!\n");
		return;
	}
  p->q[p->size] = data;
	p-> size++;
}

