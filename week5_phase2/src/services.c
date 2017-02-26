int GetPid(void){
  int pid;

  asm("int $0x30; movl %%eax, %0"
    : "=g" (pid)
    :
    : "eax"   
  );
/*  asm("pushl %%eax;
      int $0x64;
      movl %%eax, %0; 
      popl %%eax"
      : "=g" (pid)
      :
      );
*/
return pid;
}

void Sleep(int sleep_amount){
  asm("movl %0, %%eax; int $0x31"
    :
    : "g" (sleep_amount)
    : "eax"
    
  );
/*  asm("pushl %%eax;
      int $0x65;
      movl %%eax, %0;
      popl %%eax"
      : "=g" (sleep_amount)
      :
      );
*/
}
