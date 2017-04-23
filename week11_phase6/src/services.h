// services.h, 159

#ifndef __SERVICES_H__
#define __SERVICES_H__

int GetPid(void);
void Sleep(int);
int SemAlloc(int);
void SemWait(int);
void SemPost(int);
void SysPrint(char *);
int PortAlloc(void);
void PortWrite(char *, int);
void PortRead(char *, int);
//phase 6
void FSfind(char *, char *, char *);
int FSopen(char *, char *);
void FSread(int, char *);
void FSclose(int);
#endif
