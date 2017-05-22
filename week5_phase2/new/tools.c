// tools.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int size) {
  while(size--) *p++ = 0; // set where p points to to 0 and increment p
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
  int i, data = 0;
  
  if(p->size == 0){
    return data;
  } else {
    data = p->q[0];
    p->size--;
  }
  
  for(i=0; i<Q_SIZE-1; i++){
    p->q[i] = p->q[i+1];
  }
  return data;
}

// enqueue integer to next available slot in array, size is index
void EnQ(int data, q_t *p) {

  if(p->size == Q_SIZE){
    cons_printf("Kernel Panic: queue is full, cannot EnQ!\n");
    breakpoint();
  }
  p->q[p->size] = data; // add data into the array index by the size of the queue
  p->size++;  // increment the size of the queue
}
