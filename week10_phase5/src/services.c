// services.c, 159
#include "data.h"

int GetPid(void){ // function receives no arguments, but return an integer
	int pid;

	asm("pushl %%eax;
		int $0x64;
		movl %%eax, %0; 
		popl %%eax"
		: "=g" (pid)
		:
	);
	return pid;
}

void Sleep(int sleep_amount){ // function receives arguments, return an integer
	asm("pushl %%eax;
		int $0x65;
		movl %0, %%eax;
		popl %%eax"
		:
		: "g" (sleep_amount)	//when having an input, e.g., # of seconds to sleep, the input line will b : "g" (seconds)
	);
}
int SemAlloc(int passes){
  int sid;
  asm("pushl %%eax;
    pushl %%ebx;
    movl %1, %%eax; 
    int $0x66;
    movl %%ebx, %0; 
    popl %%ebx;
    popl %%eax;"
    : "=g" (sid)
    : "g" (passes)
  );
  return sid;
}

void SemWait(int sid){
    asm("pushl %%eax;
      int $0x67;
      movl %%eax, %0;
      popl %%eax"
      :
      : "g" (sid)
   );

}

void SemPost(int sid){
  asm("pushl %%eax;
    int $0x68;
    movl %%eax, %0;
    popl %%eax"
    :
    : "g" (sid)
  );
}

void SysPrint(int *str){
    asm("pushl %%eax;
    movl %0, %%eax;
    int $0x69; 
    popl %%eax"
    :
    : "g" (str)
    );
}

int PortAlloc(void){
  int port_num;
  asm("pushl %%eax;
  int $0x6A;
  movl %%eax, %0;
  popl %%eax"
  : "=g" (port_num)
  :
  );
  Sleep(1);
  port[port_num].write_sid = SemAlloc(port_num);
  port[port_num].read_sid = SemAlloc(port_num);
  port[port_num].read_q.size = 0;
  return port_num;
}

void PortWrite(char *p, int port_num){
  while(!p && *p != '\r'){
    SemWait(port_num);
    asm("pushl %%eax;
       pushl %%ebx;
       movl %1, %%eax; 
       int $0x6B;
       movl %%ebx, %0; 
       popl %%ebx;
       popl %%eax;"
       :
       : "g" (p), "g" (port_num)
    );
    p++;
  }
  return;
}

void PortRead(char *p, int port_num){
  int size = 0;
  while(p != '\r' || size != BUFF_SIZE -1){
    asm("pushl %%eax;
      pushl %%ebx;
      movl %1, %%eax; 
      int $0x6C;
      movl %%ebx, %0; 
      popl %%ebx;
      popl %%eax;"
      :
      : "g" (p), "g" (port_num)
    );
    p++;
    size++;
  } // end of forever loop
  *p = '\0'; // null-terminate str, overwrite \r
  return;
}
