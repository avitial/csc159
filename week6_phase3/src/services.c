// services.c, 159

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
		movl %%eax, %0;
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
