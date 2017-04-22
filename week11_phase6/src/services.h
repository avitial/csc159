// services.h, 159

#ifndef __SERVICES_H__
#define __SERVICES_H__

int GetPid(void);
void Sleep(int);
int SemAlloc(int);
int SemWait(int);
int SemPost(int);
int SysPrint(char *);

//phase 6
void FSfind(char *name, char *cwd, char *data);
int FSopen(char *name, char *cwd);
void FSread(int fd, char *data);
void FSclose(int fd);
#endif
