/* sample.c - Sample Main program for SPEDE - Nov 2005, bjw */
#include <spede/stdio.h>
#include <spede/flames.h>

void DisplayMsg(int i){
  printf("%d Hello world %d ECS\n", i, 2 * i);
  cons_printf("--> Hello world <--\nCPE/CSC"); // Target printf
};

int main(void){
  long i;
  int j = 111;
  
  i = 128;

  for(j=0; j<=4; j++){ 
    DisplayMsg(i);
    i++;
  }
  return 0;
} // end main()
