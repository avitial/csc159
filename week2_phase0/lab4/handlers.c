/* Name: Avitia, Luis E
 * handlers.c, Phase 0, Exercise 4 -- Timer Event
 */

#include "spede.h"
#define name_len (unsigned)strlen(my_name) // length of my_name

char my_name[] = "Luis E Avitia";
int i = 0;  // array index val
int j = 0;
int tick_count = 0;  // cnt # of timer events

unsigned short *char_p = (unsigned short *)0xB8000 + 12 * 80 + 35; // video mem ptr

void TimerHandler(){
	if(tick_count++ % 75 == 0){ // incr tick_count, loop every .75 seconds
		*char_p = 0xf00 + my_name[i]; // ptr to display i-th char in my_name + 0xf00 (color mask)
		char_p++; // incr ptr (cursor moves to next video mem location)
		i++; // incr i

		if(i == name_len + 1){ // check len of my_name
			i = 0;
			char_p = (unsigned short *)0xB8000 + 12 * 80 + 35; // rst ptr
			for(j=0; j<=name_len + 1; j++){ // erase display
				*char_p = ' ' + 0xf00; // replace char with space
				char_p++; // incr ptr
			}
			char_p = (unsigned short *)0xB8000 + 12 * 80 + 35; // rst ptr
		}
	}
	outportb(0x20, 0x60);  // 0x20 is PIC control, reg, 0x60 dismisses IRQ 0
}
