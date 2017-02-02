//************************************************************
// Name: Avitia, Luis E
// handlers.c, Phase 0, Exercise 4 -- Timer Event
//************************************************************

#include "spede.h"
#define space_val (' ' + 0xf00)

char my_name[] = "Luis E Avitia";
int i = 0;  // array index val for my_name
int j = 0;  // to replace chars written with spaces
int tick_count = 0;  // count # of timer events
int name_len = (unsigned)strlen(my_name); // lenght of my_name

// 2-byte (unsigned short) ptr points to video memory location
///assume screen has 24 rows, 80 columns, upper-left corner (b8000)
unsigned short *char_p_origin = (unsigned short *)0xB8000 + 12 * 80 + 35; // video mem pointer to retain initial location
unsigned short *char_p = (unsigned short *)0xB8000 + 12 * 80 + 35; // video mem pointer

void TimerHandler(){
	if(tick_count++ % 75 == 0){ // increment tick_count, loop in every .75 seconds
		*char_p = 0xf00 + my_name[i]; // pointer location to display i-th char in my_name + 0xf00 (color mask)
		char_p++; // incr pointer location (cursor moves to next video memory location)
		i++;  // incr i (array index for my_name)

		if(i == name_len + 1){ // if i equals my_name lenght
			i = 0;  // reset i
			char_p = char_p_origin; // reset pointer to origin
			for(j=0; j<=name_len + 1; j++){ // loop to replace chars on screen
				*char_p = space_val; // replace char with space
				char_p++; // incr pointer location
			}
			char_p = char_p_origin; // reset pointer location
		}
	}
  /*incr tick_count
    if tick_count is 75{   // every .75 seconds
      incr i (array index for my_name)
      incr char_p 
      if(i equals the length of my_name){
         i rolls back to 0
         char_p rolls back to location/address of beginning location on screen again
         loop on j to erase all chars shown, by char_p[j] =' ' (need no color mask 0xf00)
      }
    }
   */
  // dismiss timer event (IRQ 0), otherwise, new event from timer 
  // won't be recognized by CPU since circuit uses edge-trigger flipflop
  outportb(0x20, 0x60);  // 0x20 is PIC control, reg, 0x60 dismisses IRQ 0
}
