//************************************************************
// handlers.c, Phase 0, Exercise 4 -- Timer Evenr
//************************************************************
#include "spede.h"

// 2-byte (unsigned short) ptr points to video memory location
// assume screen has 24 rows, 80 columns, upper-left corner (b8000)
char my_name[] = "Luis E Avitia";
char erase[] = "             "; 
int i = 0; // which char in my name
int j = 0;
int tick_count = 0;  // count # of timer events

// position video mem location to show 1st char in name
unsigned short *char_p = (unsigned short *)0xB8000 + 12 * 80 + 35; // video mem

void TimerHandler(){
//  outportb(0x20, 0x60);

  if(tick_count++ % 75 == 0){
    *char_p = 0xf00 + my_name[i];
    char_p++;
    i++;

    if(i == (unsigned)strlen(my_name)+1){
      i = 0; 
      char_p = (unsigned short *)0xB8000 + 12 * 80 + 35;
      
      for(j=0; j<=(unsigned)strlen(my_name)+1; j++){
        *char_p = erase[j];
        char_p++;
      }
      
      char_p = (unsigned short *)0xB8000 + 12 * 80 + 35;
    }
  }
  //  use char_p to show i-th char in name + 0xf00 (color mask)
  /*incr tick_count
    if tick_count is 75{   // every .75 seconds
      incr i (array index for my_name)
      incr char_p (cursor moves to next video memory location)
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
