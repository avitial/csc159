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
void SysPrintHandler(char *);
void PortWriteOne(int);
void PortReadOne(int);
void PortHandler(void);
void PortAllocHandler(int *);
void PortWriteHandler(char, int);
void PortReadHandler(char *, int);
//phase6
void FSfindHandler(void);
void FSopenHandler(void);
void FSreadHandler(void);
int FScanAccessFD(int, int);
int FSallocFD(int);
dir_t *FSfindName(char *);
dir_t *FSfindNameSub(char *, dir_t *);
void FSdir2attr(dir_t *, attr_t *);
void FScloseHandler(void);

#endif
