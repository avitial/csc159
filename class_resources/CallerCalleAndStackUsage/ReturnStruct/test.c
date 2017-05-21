// test.c
// return type is a typedef struct (return a struct (addr))

// #include <spede/stdio.h>
// #include <spede/flames.h>

#include <stdio.h>

typedef struct {
   int m, n;
   char str[13];
} stuff_t;

void Sub1() {
   int i = 300;
   printf("i is %d\n", i);
}

void Sub2() {
   int j;
   printf("j is %d\n", j);
}

// callee, x y are local, values copied
stuff_t DispMsg(int x, int y) {
   stuff_t z = { 100, 200, "Hello\0" };

   printf("size of stuff_t is %d\n", sizeof (stuff_t));
   printf("y is %d\n", y);
   printf("x is %d\n", x);

   return z;
}

void main() { // caller
   int a, b;
   stuff_t c;

   Sub1();
   Sub2();

   a=11;               // assigning values
   b=22;

   c = DispMsg(a, b);  // DispMsg returns stuff_t

   printf("stuff-n is %d\n", c.n);
}
