/* Team Name: Gid-OS
 * events.h, Phase 1 -- Time Sharing and Context Switches
 */

#ifndef __EVENTS_H__
#define __EVENTS_H__


#define K_CODE 0x08  // kernel code segment # (register) (32-bit segment code)
#define K_DATA 0x10  // kernel data segment # (register) (32-bit segment code)
#define K_STACK_SIZE 16384  // kernel runtime stack byte size

#define TIMER_EVENT 32 // 32-bit long

#ifndef ASSEMBLER // skip below if ASSEMBLER defined (from an assembly code)
                  // since below is not in assembler syntax
__BEGIN_DECLS

#include "types.h"  // for 'TF_t' below

void TimerEvent(void);  // defined in events.S
void Loader(TF_t *);  // coded in events.S

__END_DECLS

#endif  // ifndef ASSEMBLER

#endif  // ifndef __EVENTS_H_