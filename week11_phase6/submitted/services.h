// services.h, 159

#ifndef __SERVICES_H__
#define __SERVICES_H__

// services for time sharing and context switching (phase 1)
int GetPid(void);
// services for kernel services (phase 2)
void Sleep(int);
// services for semaphores IPC (phase 3)
int SemAlloc(int);
void SemWait(int);
void SemPost(int);
// services for device driver: "busy poll" (phase 4)
void SysPrint(char *);
// services for device driver: interrupt driven (phase 5)
int PortAlloc(void);
void PortWrite(char *, int);
void PortRead(char *, int);
// services for file services (phase 6)
void FSfind(char *, char *, char *);
int FSopen(char *, char *);
void FSread(int, char *);
void FSclose(int);
// services for process life cycle (phase 7)

#endif
