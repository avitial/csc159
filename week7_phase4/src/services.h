// services.h, 159

#ifndef __SERVICES_H__
#define __SERVICES_H__

int GetPid(void);
void Sleep(int);

int SemAlloc(int);
int SemWait(int);
int SemPost(int);

int SysPrint(char *);

int PortAlloc(void);
void PortWrite(char *, int);
void PortRead(char *, int);

#endif
