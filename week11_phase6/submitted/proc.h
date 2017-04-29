// proc.h, 159

#ifndef __PROC_H__
#define __PROC_H__

void Init(void);      // PID 1, eternal, never preempted
void UserProc(void);  // PID 2, 3, ...
void Vehicle(void);   // PID for new vehicle processes
void TermProc(void);  // PID for terminal
void TermCd(char *, char *, int);  // PID to change directory
void TermCat(char *, char *, int); // PID to concatenate
void TermLs(char *, int);	   // PID to list files
void Attr2Str(attr_t *, char *);   // PID to change attribute to string

#endif
