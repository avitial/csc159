nclude "spede.h"
#include "types.h"
#include "data.h"

// clear DRAM data blocks by filling zeroes
void MyBzero(char *p, int size) {
   while(size--) *p++ = 0; // one statement as per Chang's feedback
}

// dequeue, return 1st integer in array, and move all forward
// if queue empty, return 0
int DeQ(q_t *p) { // return 0 if q[] is empty
  int i, data = 0; 
	
  if(p->size == 0){
    return data;
  } else{
    data = p->q[0];
    p->size--;
    
   for(i=0; i<Q_SIZE-1; i++){
      p->q[i] = p->q[i+1];  
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
  p->size++; //= p->size + 1;
}

//return the length (num of bytes) of a string p points
int MyStrlen(char *p){
  int len = 0;
  while(*p != 0){
    p++;
    len++;
  }
  return len;
}

void MyStrcat(char *dst, char *addendum){
  int str_len, add_len;
  int j = 0;
  str_len = MyStrlen(dst);
  dst += str_len;
  add_len = MyStrlen(addendum);
  while(j < add_len){
    dst[str_len] = addendum[j];
    str_len++;
    j++;
  }
  dst[str_len] = '\0';
}

int MyStrcmp(char *p, char *q, int len){ //two ways to do it
  while(len && p != '\0' && *p == *q){
  p++;
  q++;
  len--;
  }
  if(len == 0 || (*p == '\0' && *q =='\0')){
    return 1;
  }
  return 0;
}

void MyStrcpy(char *dst, char *src){
  char ch;
  ch = *src;
  while(ch != '\0'){
    *dst = ch;
    dst++;
    src++;
    ch = *src;
  }
  *dst = '\0';
}

void MyMemcpy(char *dst, char *src, int size){
  while(size--){
    *dst++=*src++;
  }
}
