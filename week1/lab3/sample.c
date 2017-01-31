/* sample.c - Sample Main program for SPEDE - Nov 2005, bjw */

#include <spede/stdio.h>
#include <spede/flames.h>

int main(void){
  int i=0;
  for(i; i<=200; i++){
    do{
      printf("%d Hello world %d \nECS", i, 2*i);
      cons_printf("--> Hello world <--\nCPE/CSC"); // Target printf
    } while(i>=111 && i<=115);
  }
  return 0;
} // end main()
