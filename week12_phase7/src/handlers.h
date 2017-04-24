// handlers.h, 159

#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include "types.h"   // need definition of 'func_ptr_t' below

// Time Sharing and Context Switches (phase 1)
void NewProcHandler(func_ptr_t);
void TimerHandler(void);
// Kernel Services(phase 2)
void SleepHandler(int);
void GetPidHandler(void);
// Semaphores IPC (phase 3)
void SemAllocHandler(int);
void SemWaitHandler(int);
void SemPostHandler(int);
// Device Driver: Busy Poll Printing (phase 4)
void SysPrintHandler(char *);
// Device Driver: Interrupt Driven (phase 5)
void PortWriteOne(int);
void PortReadOne(int);
void PortHandler(void);
void PortAllocHandler(int *);
void PortWriteHandler(char, int);
void PortReadHandler(char *, int);
// File Services (phase 6)
void FSfindHandler(void);
void FSopenHandler(void);
void FSreadHandler(void);
int FScanAccessFD(int, int);
int FSallocFD(int);
dir_t *FSfindName(char *);
dir_t *FSfindNameSub(char *, dir_t *);
void FSdir2attr(dir_t *, attr_t *);
void FScloseHandler(void);

//phase 7
void ForkHandler(char *, int *);
void WaitHandler(int *);
void ExitHandler(int);

#endif
