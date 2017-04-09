// services.h, 159
#include "data.h"

#ifndef __SERVICES_H__
#define __SERVICES_H__
int GetPid(void);
void Sleep(int);

int SemAlloc(int);
int SemWait(int);
int SemPost(int);
void SysPrint(int); 
int PortAlloc(void);
void PortWrite(char, int);
void PortRead(char, int);

#endif
