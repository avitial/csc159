//************************************************************
// handlers.c, Phase 0, Exercise 4 -- Timer Evenr
//************************************************************
#include "spede.h"

// 2-byte (unsigned short) ptr points to video memory location
// assume screen has 24 rows, 80 columns, upper-left corner (b8000)
char my_name[] = "Luis E Avitia"; 
int i = 0; // which char in my name
int tick_count = 0;  // count # of timer events

// position video mem location to show 1st char in name
unsigned short *char_p = (unsigned short *) 0xB800+12*80+34; // video mem

void TimerHandler(){
  if tick_count is 0:
    use char_p to show i-th char in name + 0xf00 (color mask)

  incr tick_count
    if tick_count is 75{   // every .75 seconds
      incr i (array index for my_name)
      incr char_p (cursor moves to next video memory location)
      if(i equals the length of my_name){
         i rolls back to 0
         char_p rolls back to location/address of beginning location on screen again
         loop on j to erase all chars shown, by char_p[j] =' ' (need no color mask 0xf00)
      }
    }
  // dismiss timer event (IRQ 0), otherwise, new event from timer 
  // won't be recognized by CPU since circuit uses edge-trigger flipflop
    outportb(0x20, 0x60);  // 0x20 is PIC control, reg, 0x60 dismisses IRQ 0
}
