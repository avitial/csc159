// handlers.h, 159

#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include "types.h"   // need definition of 'func_ptr_t' below

void NewProcHandler(func_ptr_t);
void TimerHandler(void);
void SleepHandler(int);
void GetPidHandler(void);
void SemAllocHandler(int);
void SemWaitHandler(int);
void SemPostHandler(int);

#endif
