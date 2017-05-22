// handlers.h, 159

#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include "types.h"   // need definition of 'func_ptr_t' below

// Time Sharing and Context Switches (phase 1)
void NewProcHandler(func_ptr_t);
void TimerHandler(void);
// Kernel Services (phase 2)
void GetPidHandler(void);
void SleepHandler(int);

#endif
