// services.c, 159

int GetPid(void){             // function receives no arguments, but return an integer
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
		movl %%eax, %0;
		popl %%eax"
		:
		: "g" (sleep_amount)      //when having an input, the input line will b : "g" (seconds)
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
      movl %0, %%eax;
      popl %%eax"
      :
      : "g" (sid)
   );
}

void SemPost(int sid){
  asm("pushl %%eax;
    int $0x68;
    movl %0, %%eax;
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
//phase 6
void FSfind(char *name, char *cwd, char *data){ //find CWD/name, return attrdata
  char tmp[BUFF_SIZE];
  MyStrcpy(tmp, cwd);
  MyStrcat(tmp, name);
  asm("pushl %%eax
       pushl %%ebx
       movl %0, %%eax
       movl %1, %%ebx
       int $0x6D
       popl %%eax
       popl %%ebx"
       :
       :"g" ((int)tmp, "g" ((int)data)   //might be wrong
  );
}

int FSopen(char *name, char *cwd){ //alloc FD to open CWD/name
  int fd;
  char tmp[BUFF_SIZE];
  MyStrcpy(tmp, cwd);
  MyStrcat(tmp, "/");
  MyStrcat(tmp, name);
  
  asm("pushl %% eax
       pushl %%ebx
       movl %1, %%eax
       int 0x6E
       movl %%ebx, %0
       popl %%ebx
       popl %%eax"
       : "=g" (fd)
       : "g" ((int)tmp)
       );
    return fd;
}

void FSread(int fd, char *data){ //read FD into data buffer
  


}

void Fsclose(int fd){ //close allocated fd (FD)

}
