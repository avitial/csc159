# printer_msg.s, make printer print out a hello msg
# you must program this to make it work on both terminals
# add this to the bin/ directory of the file service

.text
_start:
   ...   # mov addr of 'msg' to a register (this is linker given, not our virtual addr)
   ...   # subtract the starting addr of code from it, we get the distance between them
   ...   # add 1G to the this distance, we get the correct virtual address
   ...   # save a copy of this calculated result onto stack
   ...   # and call the SysPrint intr # to issue printer printing

   ...   # pop from stack to eax as the exit #
   ...   # and call the exit intr

.data
   ...     # declare a msg such as: Hello... (run demo to view how it should be)

