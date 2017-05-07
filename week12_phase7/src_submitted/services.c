// services.c, 159
#include "data.h"
#include "tools.h"

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
    movl %0, %%eax;
    int $0x65;
    popl %%eax"
    :
    : "g" (sleep_amount) //when having an input, the input line will b : "g" (seconds)
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
      movl %0, %%eax;
      int $0x67;
      popl %%eax"
      :
      : "g" (sid)
  );
}
  
void SemPost(int sid){
  asm("pushl %%eax;
    movl %0, %%eax;
    int $0x68;
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
  port[port_num].write_sid = SemAlloc(Q_SIZE);
  port[port_num].read_sid = SemAlloc(0);
  port[port_num].read_q.size = 0;

  return port_num;
}

void PortWrite(char *p, int port_num){
  
  while(*p != '\0'){
    SemWait(port[port_num].write_sid);
    asm("pushl %%eax;
      pushl %%ebx;
      movl %0, %%eax;
      movl %1, %%ebx;
      int $0x6B;
      popl %%ebx;
      popl %%eax"
      :
      : "g" ((int)*p), "g" (port_num)
    );
    p++;
  }
}

void PortRead(char *p, int port_num){
  int size = 0;

  while(1){
    SemWait(port[port_num].read_sid);
    asm("pushl %%eax;
      pushl %%ebx;
      movl %0, %%eax;
      movl %1, %%ebx;
      int $0x6C;
      popl %%ebx;
      popl %%eax"
      :
      : "g" ((int)p), "g" (port_num)
    );
    
    if(*p == '\r') break; // if char is newline
    p++;
    size++;

    if(size == BUFF_SIZE-1) break; // if size equals buffsize
  } // end of forever loop
  *p = '\0'; // null-terminate str, overwrite \r
}

void FSfind(char *name, char *cwd, char *data){ //find CWD/name, return attrdata
  char tmp[BUFF_SIZE];
  
  MyStrcpy(tmp, cwd);
  MyStrcat(tmp, name);
  PortWrite("tmp is ", 0);
  PortWrite(tmp, 0);
  asm("pushl %%eax;
       pushl %%ebx;
       movl %0, %%eax;
       movl %1, %%ebx;
       int $0x6D;
       popl %%ebx;
       popl %%eax"
       : 
       :"g" ((int)tmp), "g" ((int)data)   //might be wrong
  );
}

int FSopen(char *name, char *cwd){ //alloc FD to open CWD/name
  int fd;
  char tmp[BUFF_SIZE];

  MyStrcpy(tmp, cwd);
  MyStrcat(tmp, "/");
  MyStrcat(tmp, name);
  asm("pushl %% eax;
       pushl %%ebx;
       movl %1, %%eax;
       int $0x6E;
       movl %%ebx, %0;
       popl %%ebx;
       popl %%eax"
       : "=g" (fd)
       : "g" ((int *)tmp)
  );
  return fd;
}

void FSread(int fd, char *data){ //read FD into data buffer
  asm("pushl %%eax;
       pushl %%ebx;
       movl %0, %%eax
       movl %1, %%ebx;
       int $0x6F;
       popl %%ebx;
       popl %%eax"
      : 
      : "g" (fd), "g" ((int *)data) 
  );
}

void FSclose(int fd){ //close allocated fd (FD)
  asm("pushl %%eax;
      movl %%eax, %0;
      int $0x70;
      popl %%eax"
      :
      : "g" (fd)
  );
}

int Fork(char *p){       // parent creates child, child PID returns
	int cpid;

	asm("pushl %% eax;
		movl %0, %%eax;
		int $0x71;
		popl %%eax"
		: "=g" (cpid)
		: "g" ((int *)p)
	);
	return cpid;
}

int Wait(void){          // parent process waits exit_num from exiting child
	int exit_num;

	asm("pushl %%eax;
		int $0x72;
		movl %%eax, %0;
		popl %%eax"
		: "=g" (exit_num)
		:
	);
	return exit_num;
}

void Exit(int exit_num){ // process exits, send exit_num to parent
	asm("pushl %%eax;
		movl %0, %%eax;
		int $0x73;
		popl %%eax"
		:
		: "g" (exit_num)
	);
}