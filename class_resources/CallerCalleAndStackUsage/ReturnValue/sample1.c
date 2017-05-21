// sample1.c, showing caller-callee in C
// will be deassembled into sample2.asm
//
// edit this file 1st, then use SPEDE tool:
//    ~spede2/bin/spede-mkmf -q
//    make sample1.asm

#include <spede/stdio.h>
#include <spede/flames.h>

// callee, x y are local, values copied
int DispMsg(int x, int y) {
   int z = 5; // z is also local

   printf("x is %d\n", x);
   printf("y is %d\n", y);

   return z;
}

void main() // caller {
   int a, b, c; // local vars

   a=1;               // assigning values
   b=2;
   c = DispMsg(a, b); // call subroutine
   printf("d is %d\n", c);
}
