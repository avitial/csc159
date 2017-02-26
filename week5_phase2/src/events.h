// events.h of events.S
// prototypes those coded in events.S

#ifndef __EVENTS_H__
#define __EVENTS_H__

// #include <spede/machine/pic.h> // shouldn't need this anymore

#define K_CODE 0x08         // kernel code segment # (register)
#define K_DATA 0x10         // kernel data segment # (register)
#define K_STACK_SIZE 16384  // kernel runtime stack byte size
#define GETPID_EVENT 0x64 
#define SLEEP_EVENT 0x65
#define TIMER_EVENT 0x20// 32      // IDT entry #32, aka IRQ0, for timer device

#ifndef ASSEMBLER  // skip below if ASSEMBLER defined (from an assembly code)
                   // since below is not in assembler syntax
__BEGIN_DECLS

#include "types.h"          // for 'TF_t' below

extern void TimerEvent(void);      // coded in events.S, assembler won't like this syntax
extern void Loader(TF_t *);        // coded in events.S
extern void GetPidEvent();
extern void SleepEvent();
__END_DECLS

#endif // ifndef ASSEMBLER

#endif // ifndef __EVENTS_H__

