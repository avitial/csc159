int GetPid(void){
  int pid;
/*
  asm("int $0x64; movl %%eax, %0"
    : "=g" (pid)
    :
    : "eax");
*/
  asm("pushl %%eax;
      int $0x64;
      movl %%eax, %0; 
      popl %%eax"
      : "=g" (pid)
      :
      );
  return pid;
}

int Sleep(int pid, int sleep_amount){
  //int pid
  //asm("movl %0, %%eax; int $0x65"
/*  asm("movl %0, %%eax; int $0x65"
    //: "=g" (pid)
    :
    : "g" (sleep_amount)
    : "eax");
*/  asm("pushl %%eax;
      int $0x65;
      movl %%eax, %0;
      popl %%eax"
      : "=g" (pid)
      : "g" (sleep_amount)
      );
  return pid;
}
