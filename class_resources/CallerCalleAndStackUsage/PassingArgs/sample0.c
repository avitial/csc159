// sample0.c, showing caller-callee in C
// will be deassembled into sample.asm
//
// edit this file 1st, then use SPEDE tool:
//    ~spede2/bin/spede-mkmf -q
//    make sample0.asm

#include <spede/stdio.h>
#include <spede/flames.h>

// callee, d e f are also local, values copied
DispMsg(int d, int e, int f) {
   printf("d is %d\n", d);
   printf("e is %d\n", e);
   printf("f is %d\n", f);
}

void main() // caller {
   int a, b, c; // local vars

   a=1; b=2; c=3; // values assigned

   DispMsg(a, b, c); // call subroutine
}

