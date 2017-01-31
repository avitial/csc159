/* sample.c - Sample Main program for SPEDE - Nov 2005, bjw */
#include <spede/stdio.h>
#include <spede/flames.h>

void DisplayMsg(int i){
  printf("%d Hello world %d ECS\n", i, i);
  cons_printf("--> Hello world <--\nCPE/CSC"); // Target printf
};

int main(void){
  long i;
  i = 111;

  for(i; i<=115; i++){ 
    DisplayMsg(i); 
  }
  return 0;
} // end main()
