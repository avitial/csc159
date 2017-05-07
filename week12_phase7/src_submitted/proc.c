// proc.c, 159
// all processes are coded here
// processes do not use kernel space (data.h) or code (handlers, tools, etc.)
// all must be done thru system service calls

#include "services.h"
#include "spede.h"      // cons_xxx below needs
#include "data.h"       // current_pid needed below
#include "proc.h"       // prototypes of processes
#include "handlers.h"
#include "tools.h"

// Init PID 1, always ready to run, never preempted
void Init(void) {
  int i;
  char key;
  char str[] = " Hello, World! Team GidOS: Eloisa Esparza and Luis Avitia\n\r";
  
  while(1){
    if(cons_kbhit()){       // if a key is pressed on Target PC
      key = cons_getchar(); // get the key
      
      switch(key){          // switch by the key obtained {
        case 'p':
          SysPrintHandler(str); // call SysPrintHandler to print
          break;
        case 'b':
          breakpoint();     // go into gdb
          break;
        case 'q':
          exit(0);          // quit program
      }
    }

    for(i=0; i<LOOP; i++){  // loop for LOOP times to cause approx 1 second of delay
      asm("inb $0x80");     // call asm("inb $0x80") which delay .6 microsecond
    }
  }
}

// PID 2, 3, 4, etc. mimicking a usual user process
void UserProc(void) {
  int pid, sleep_amount;
  pid = GetPid();
  
  while(1){
    sleep_amount = pid;
    cons_printf("%d..", pid); // show on Target PC: "%d..", current_pid
    Sleep(sleep_amount);
  }
}

void Vehicle(void){           // phase 3 tester (multiple processes)
  int i, pid;
  int sleep_amount = 1;
  
  if(vehicle_sid == -1){
    vehicle_sid = SemAlloc(3); // max passes 3
  }
  pid = GetPid();

  while(1){
    ch_p [pid*80+45]= 0xf00 + 'f';  // show i'm off the bridge
    
    for(i =0; i<LOOP; i++){ // spend a sec in RUN state
      asm("inb $0x80");
    }
    SemWait(vehicle_sid);   // ask for a pass
    ch_p[pid*80+45] = 0xf00 + 'o'; // show i'm on the bridge
    Sleep(sleep_amount);    // sleep for sleep_amount secs
    SemPost(vehicle_sid);   // return the pass
  }
}

void TermProc(void){
  int my_port, len, i, exit_num;
  char login_str[BUFF_SIZE], passwd_str[BUFF_SIZE], cmd_str[BUFF_SIZE], cwd[BUFF_SIZE];
  my_port = PortAlloc(); // init port device and port_t data associated

  while(1){   // 1st while
    while(1){ // 2nd while
      PortWrite("Please enter your login: ", my_port);     // \r also!
      PortRead(login_str, my_port);
      PortWrite("Please enter your password: ", my_port);  // \r also!
      PortRead(passwd_str, my_port);
      
      if(MyStrlen(login_str) == 0 || login_str[0] == '\n') continue;
      len = MyStrlen(login_str);
      if(len != MyStrlen(passwd_str)){
        PortWrite("Invalid password length, please try again.\n\r", my_port);	
	continue;
      }
      i = 0;
      if(login_str[i]!=passwd_str[(len-1)-i]){
        PortWrite("Invalid password, please try again.\n\r", my_port);
	continue; 
      } else{
          i++;
      }
      MyStrcpy(cwd, "/");
	  exit_num = 0;
	  //cwd[0] = '/';
      //MyBzero((char *)&cwd[1], sizeof(cwd)-1);
      break;
    } // end of 2nd while 
    while(1){ // 3rd while
      PortWrite("Please enter command string (pwd, ls, cd, cat, echo, exit): ", my_port);
      PortRead(cmd_str, my_port);

      if(MyStrlen(cmd_str)==0) continue;
      len = MyStrlen(cmd_str);

      if(MyStrcmp(cmd_str, "exit\0", len)) break;

      if(MyStrcmp(cmd_str, "pwd\0",len)){
        PortWrite(cwd, my_port);
		PortWrite("\n\r", my_port);
      } else if (MyStrcmp(cmd_str, "cd ", 3)){
        TermCd(&cmd_str[3], cwd, my_port);
      } else if(MyStrcmp(cmd_str, "ls\0",len)){
        TermLs(cwd, my_port);
      } else if(MyStrcmp(cmd_str, "cat ", 4)){
        TermCat(&cmd_str[4], cwd, my_port);
	  } else if (MyStrcmp(cmd_str, "echo\0", 4)){
		  PortWrite(cmd_str, my_port);
		  TermBin(&cmd_str[4], cwd, my_port, &exit_num);
	  }
	  else{
        PortWrite("Command not recognized!\n\r", my_port);
		//exit_num = TermBin(char *name, char *cwd, int my_port);
	  }
    } // end of 3rd while
  } // end of 1st while
}

void TermCd(char *name, char *cwd, int my_port){
  char attr_data[BUFF_SIZE];
  attr_t *attr_p;
  int str_len = MyStrlen(name);
  
  if(str_len == 0) return;
  if(MyStrcmp(name, ".\0", str_len)) return; // current directory, no changes
  if(MyStrcmp(name, "/\0", str_len) || MyStrcmp(name, "..\0", str_len)){
    //cwd[0] = '/';
    MyStrcpy(cwd, "/");
    //MyBzero((char *)&cwd[1], sizeof(cwd)-1);
    return;
  }
  FSfind(name, cwd, attr_data);

  if(MyStrlen(attr_data) == 0){
    PortWrite(attr_data, my_port);
    PortWrite("Directory not found!\n\r", my_port);
    return;
  }
  attr_p = (attr_t*)attr_data;
  
  if(attr_p->mode == MODE_FILE){
   PortWrite("Cannot cd a file!\n\r", my_port);
   return;
  }
  MyStrcpy(&cwd[1], name);
}

void TermCat(char *name, char *cwd, int my_port){
  char read_data[BUFF_SIZE], attr_data[BUFF_SIZE];
  attr_t *attr_p;
  int my_fd;

  FSfind(name, cwd, attr_data);
  
  if(MyStrlen(attr_data) == 0){
    PortWrite("Not found!\n\r", my_port);
    return;
  }
  attr_p = (attr_t *)attr_data; 
  
  if(attr_p->mode == MODE_DIR){
      PortWrite("Cannot cat a directory!\n\r", my_port);
      return;
  }
  my_fd = FSopen(name, cwd);

  while(1){
    FSread(my_fd, read_data); 
    if(MyStrlen(read_data) == 0){
      PortWrite(read_data, my_port);
      break;
    }
  }
  FSclose(my_fd);
}

void TermLs(char *cwd, int my_port){
  char ls_str[BUFF_SIZE], attr_data[BUFF_SIZE];
  attr_t *attr_p;
  int my_fd;

  FSfind("", cwd, attr_data);

  if(MyStrlen(attr_data) == 0){
    PortWrite("Not found\n\r", my_port);
    return;
  }
  attr_p = (attr_t *)attr_data;

  if(attr_p->mode == MODE_FILE){
    PortWrite("Cannot ls a file\n\r", my_port);
    return;
  }
  my_fd = FSopen("", cwd);
  
  while(1){
    FSread(my_fd, attr_data);
    
    if(MyStrlen(attr_data) == 0){
      break;
    }
    attr_p = (attr_t *)attr_data;
    Attr2Str(attr_p, ls_str);
    PortWrite(ls_str, my_port);
  }
  FSclose(my_fd);
}

void TermBin(char *name, char *cwd, int my_port, int *exit_num){
	char attr_data[BUFF_SIZE];
	attr_t *attr_p;
	int cpid;

	FSfind(name, cwd, attr_data);
	if(attr_p->mode == MODE_EXEC){
		cpid = Fork(attr_p->data);
	} else {
		PortWrite("Error message: no attribute and no executable found\n\r", my_port);
	}
	PortWrite((char *)&cpid, my_port);
	Wait();
}

void Attr2Str(attr_t *attr_p, char *str){
  char *name = (char *)(attr_p + 1);
  sprintf(str, "     - - - -    SIZE %4d    NAME  %s\n\r", attr_p->size, name);

  if ( A_ISDIR(attr_p->mode) ) str[5] = 'D';          // mode is directory
  if ( QBIT_ON(attr_p->mode, A_ROTH) ) str[7] = 'R';  // mode is readable
  if ( QBIT_ON(attr_p->mode, A_WOTH) ) str[9] = 'W';  // mode is writable
  if ( QBIT_ON(attr_p->mode, A_XOTH) ) str[11] = 'X'; // mode is executable
}
