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
		: "g" (sleep_amount)	//when having an input, e.g., # of seconds to sleep, the input line will be: "g" (seconds)
	);
}
