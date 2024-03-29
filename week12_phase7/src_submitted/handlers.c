// handlers.c, 159

#include "spede.h"
#include "types.h"
#include "handlers.h"
#include "tools.h"
#include "data.h"
#include "proc.h"

// to create process, alloc PID, PCB, and stack space
// build TF into stack, set PCB, register PID to ready_q
void NewProcHandler(func_ptr_t p){ // arg: where process code starts
  int pid;

  if(free_q.size == 0){ // this may occur for testing
    cons_printf("Kernel Panic: no more PID left!\n");
    breakpoint();         // breakpoint() into GDB
  }
  pid = DeQ(&free_q);     // get 'pid' from free_q
  MyBzero((char *)&pcb[pid], sizeof(pcb_t));
  MyBzero((char *)&proc_stack[pid], PROC_STACK_SIZE); // use tool to clear the PCB (indexed by 'pid')
  pcb[pid].state = READY;
  EnQ(pid, &ready_q);
  pcb[pid].TF_p = (TF_t *)&proc_stack[pid][PROC_STACK_SIZE - sizeof(TF_t)]; // point TF_p to highest area in stack
// then fill out the eip of the TF
  pcb[pid].TF_p->eip = (unsigned int)p; // new process code
  pcb[pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // EFL will enable intr!
  pcb[pid].TF_p->cs = get_cs(); // duplicate from current CPU
  pcb[pid].TF_p->ds = get_ds(); // duplicate from current CPU
  pcb[pid].TF_p->es = get_es(); // duplicate from current CPU
  pcb[pid].TF_p->fs = get_fs(); // duplicate from current CPU
  pcb[pid].TF_p->gs = get_gs(); // duplicate from current CPU
  //pcb[pid].cpu_time = 0;        //pcb[pid].total_cpu_time = 0;

  if(pid>9){
    ch_p[pid*80+40]=0xf00+ (pid/10+'0');
    ch_p[pid*80+41]=0xf00+(pid%10+'0');
  } else{
    ch_p[pid*80+40]=0xf00+pid+'0';
  }
  ch_p[pid*80+43] = 0xf00 +'r';
}

void GetPidHandler(void){
  pcb[current_pid].TF_p->eax = (unsigned int)current_pid;
}

// count cpu_time of running process and preempt it if reaching limit
void TimerHandler(void){
  int i;
  current_time++;
  pcb[current_pid].cpu_time++; // upcount cpu_time of the process (PID is current_pid)

  for(i=1; i<=Q_SIZE; i++){  // phase 2
    if((pcb[i].state == SLEEP) && (pcb[i].wake_time == current_time)){
      EnQ(i, &ready_q);     // append pid to ready_q
      pcb[i].state = READY; // update proc state
      ch_p[i*80+43] = 0xf00 +'r';
    }
  }

  if(pcb[current_pid].cpu_time == TIME_LIMIT){ // if its cpu_time reaches the preset OS time limit
    pcb[current_pid].state = READY; // update/downgrade its state
    EnQ(current_pid, &ready_q);     // move it to ready_q
    ch_p[current_pid*80+43] = 0xf00 +'r';
    current_pid = 0;    // no longer runs
  }
  outportb(0x20, 0x60); // Don't forget: notify PIC event-handling done
}

void SleepHandler(int sleep_amount){
  pcb[current_pid].wake_time = (current_time + (100 * sleep_amount)); // calc future wake time in pcb
  pcb[current_pid].state = SLEEP; // update proc state
  ch_p[current_pid*80+43] = 0xf00 +'S';
  current_pid = 0; // reset current_pid
}

void SemAllocHandler(int passes){
  int sid;

  for(sid=0; sid<PROC_NUM; sid++){
    if(sem[sid].owner == 0) break;
  }
  if(sid == PROC_NUM){
    cons_printf("Kernel panic: no more semaphores left!\n");
    return;
  }
  sem[sid].passes = passes;
  sem[sid].owner = current_pid;
  MyBzero((char *)&sem[sid].wait_q, sizeof(q_t));
  pcb[current_pid].TF_p -> ebx = sid;
}

void SemWaitHandler(int sid){

  if(sem[sid].passes > 0){
    sem[sid].passes--;
    ch_p[48] = 0xf00 + sem[sid].passes + '0';
    return;
  } else{
    EnQ(current_pid, &sem[sid].wait_q);
    pcb[current_pid].state = WAIT;
    ch_p[current_pid * 80 + 43] = 0xf00 + 'W';
    current_pid = 0;
  }
}

void SemPostHandler(int sid){
  int free_pid = 0;

  if((sem[sid].wait_q.size == 0)){
    sem[sid].passes++;
    ch_p[48] = 0xf00 + sem[sid].passes + '0';
    return;
  } else{
    free_pid = DeQ(&sem[sid].wait_q);
    EnQ(free_pid, &ready_q);
    pcb[free_pid].state = READY;
    ch_p[free_pid*80+43] = 0xf00 +'r';
  }
}

void SysPrintHandler(char *str){
  int i, code;
  const int printer_port = 0x378; // I/O mapped # 0x378
  const int printer_data = printer_port + 0; // data register
  const int printer_status = printer_port + 1; // status register
  const int printer_control = printer_port + 2; // control register
  // initialize printer port (check printer power, cable, and paper)
  outportb(printer_control, 16); // 1<<4 is PC_SLCTIN
  code = inportb(printer_status); // read printer status
  
  for(i=0; i<50; i++) asm("inb $0x80"); // needs some delay
  outportb(printer_control, 4 | 8 ); // 1<<2 is PC_INIT, 1<<3 PC_SLCTIN
  
  while(*str){
    outportb(printer_data, *str); // write char to printer data
    code = inportb(printer_control); // read printer control
    outportb(printer_control, code | 1); // 1<<0 is PC_STROBE
    for(i=0; i<50; i++) asm("inb $0x80"); // needs some delay
    outportb(printer_control, code); // send original (cancel strobe)
    
    for(i = 0; i < LOOP*3; i++) { // 3 seconds at most
      code = inportb(printer_status) & 64; // 1<<6 is PS_ACK
      if(code == 0) break; // printer ACK'ed
      asm("inb $0x80"); // otherwise, wait 0.6 us, and loop
    } 

    if(i == LOOP*3) { // if 3 sec did pass (didn't ACK)
      cons_printf(">>> Printer timed out!\n");
      break; // abort printing
    }
    str++; // move to print next character
  } // while(*str)
}

void PortWriteOne(int port_num){
  char one;
  
  if(port[port_num].write_q.size == 0 && port[port_num].loopback_q.size == 0){
    port[port_num].write_ok = 1; // record missing write event
    return;
  }
  if(port[port_num].loopback_q.size != 0){
    one = DeQ(&port[port_num].loopback_q);
  } else {
    one = DeQ(&port[port_num].write_q);
    SemPostHandler(port[port_num].write_sid);
  }
  outportb(port[port_num].IO+DATA, one);
  port[port_num].write_ok = 0; // will use write event below
}

void PortReadOne(int port_num){
  char one;
  one = inportb(port[port_num].IO+DATA);
  
  if(port[port_num].read_q.size == Q_SIZE){
    cons_printf("Kernel Panic: you are typing on terminal is super fast!\n");
    return;
  }
  EnQ(one, &port[port_num].read_q);
  EnQ(one, &port[port_num].loopback_q);
  
  if(one == '\r'){
    EnQ('\n', &port[port_num].loopback_q);
  }
  SemPostHandler(port[port_num].read_sid);
}

void PortHandler(){
  int port_num, intr_type;
  
  for(port_num=0; port_num<PORT_NUM; port_num++){ // PORT_NUM equals 3 (COM Ports 2, 3 4)
    intr_type = inportb(port[port_num].IO+IIR);
    
    if(intr_type == IIR_RXRDY){
      PortReadOne(port_num);
    }
   
    if(intr_type == IIR_TXRDY || port[port_num].write_ok == 1){
      PortWriteOne(port_num);
    }
  }
  outportb(0x20, 0x63);
  outportb(0x20, 0x64);
}

void PortAllocHandler(int *eax){
  int port_num, baud_rate, divisor;
  static int IO[PORT_NUM] = {0x2f8, 0x3e8, 0x2e8};

  for(port_num=0; port_num<PORT_NUM; port_num++){
    if(port[port_num].owner == 0) break; // found one
  }

  if(port_num == PORT_NUM){
    cons_printf("Kernel Panic: no port left!\n");
    return;
  }
  *eax = port_num;
  MyBzero((char *)&port[port_num], sizeof(port_t));
  port[port_num].owner = current_pid;
  port[port_num].IO = IO[port_num];
  port[port_num].write_ok = 1;
  baud_rate = 9600;
  divisor = 115200 / baud_rate;
  outportb(port[port_num].IO+CFCR, CFCR_DLAB);
  outportb(port[port_num].IO+BAUDLO, LOBYTE(divisor));
  outportb(port[port_num].IO+BAUDHI, HIBYTE(divisor));
  outportb(port[port_num].IO+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);
  outportb(port[port_num].IO+IER, 0);
  outportb(port[port_num].IO+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
  asm("inb $0x80");
  outportb(port[port_num].IO+IER, IER_ERXRDY|IER_ETXRDY);
}

void PortWriteHandler(char one, int port_num){
  
  if(port[port_num].write_q.size == Q_SIZE){ // Q_SIZE or BUFF_SIZE???
    cons_printf("Kernel Panic: terminal is not prompting (fast enough)?\n");
    return;
  }
  EnQ(one, &port[port_num].write_q);//buffer one
  
  if(port[port_num].write_ok == 1){
    PortWriteOne(port_num);
  }
}

void PortReadHandler(char *one, int port_num){

  if(port[port_num].read_q.size == 0){
    cons_printf("Kernel Panic: nothing in typing/read buffer?\n");
    return;
  }
  *one = DeQ(&port[port_num].read_q);
}

void FSfindHandler(void){
  char *name, *data;
  attr_t *attr_p;
  dir_t *dir_p;

  name = (char *)pcb[current_pid].TF_p->eax;
  data = (char *)pcb[current_pid].TF_p->ebx;
  dir_p = FSfindName(name);

  if(! dir_p) {   // dir_p == 0, not found
    data[0] = 0;  // null terminated, not found, return
    return;
  }

  attr_p = (attr_t *)data;
  FSdir2attr(dir_p, attr_p); // copy what dir_p points to to where attr_p points to

  // should include filename (add 1 to length for null char)
  MyMemcpy((char *)(attr_p + 1), dir_p->name, MyStrlen(dir_p->name) + 1);
}

void FSopenHandler(void) {
  char *name;
  int fd;
  dir_t *dir_p;

  name = (char *)pcb[current_pid].TF_p->eax;
  fd = FSallocFD(current_pid);  // current_pid is owner of fd allocated

  if( fd == -1 ) {
    cons_printf("FSopenHandler: no more File Descriptor!\n");
    pcb[current_pid].TF_p->ebx = -1;
    return;
  }

  dir_p = FSfindName(name);
  if(! dir_p) {
    cons_printf("FSopenHandler: name not found!\n");
    pcb[current_pid].TF_p->ebx = -1;
    return;
  }
  fd_array[fd].item = dir_p;        // dir_p is the name
  pcb[current_pid].TF_p->ebx = fd;  // process gets this to future read
}

// Copy bytes from file into user's buffer. Returns actual count of bytes
// transferred. Read from fd_array[fd].offset (initially given 0) for
// buff_size in bytes, and record the offset. may reach EOF though...
void FSreadHandler(void){
  int fd, result, remaining;
  char *read_data;
  dir_t *lp_dir;

  fd = pcb[current_pid].TF_p->eax;
  read_data = (char *)pcb[current_pid].TF_p->ebx;

  if(! FScanAccessFD(fd, current_pid)) {
    cons_printf("FSreadHandler: cannot read from FD!\n");
    read_data[0] = 0;  // null-terminate it
    return;
  }
  lp_dir = fd_array[fd].item;

  if(A_ISDIR(lp_dir->mode)){  // it's a dir
    // if reading directory, return attr_t structure followed by obj name.
    // a chunk returned per read. `offset' is index into root_dir[] table.
    dir_t *this_dir = lp_dir;
    attr_t *attr_p = (attr_t *)read_data;
    dir_t *dir_p;

    if(BUFF_SIZE < sizeof( *attr_p )+2){
      cons_printf("FSreadHandler: read buffer size too small!\n");
      read_data[0] = 0;  // null-terminate it
      return;
    }

    // use current dir, advance to next dir for next time when called
    do{
      dir_p = ((dir_t *)this_dir->data);
      dir_p += fd_array[fd].offset ;

      if( dir_p->inode == END_INODE ) {
        read_data[0] = 0;  // EOF, null-terminate it
        return;
      }
      fd_array[fd].offset++;   // advance
    } while(dir_p->name == 0);

    // MyBzero() fills buff with 0's, necessary to clean buff
    // since FSdir2attr may not completely overwrite whole buff...
    MyBzero(read_data, BUFF_SIZE);
    FSdir2attr(dir_p, attr_p);
    // copy obj name after attr_t, add 1 to length for null
    MyMemcpy((char *)( attr_p + 1 ), dir_p->name, MyStrlen( dir_p->name ) + 1);

    } else {  // a file, not dir
      // compute max # of bytes can transfer then MyMemcpy()
      remaining = lp_dir->size - fd_array[fd].offset;

      if( remaining == 0 ) {
        read_data[0] = 0;  // EOF, null-terminate it
        return;
      }
      MyBzero(read_data, BUFF_SIZE);        // null termination for any part of file read
      result = remaining<100?remaining:100; // -1 saving is for last null
      MyMemcpy(read_data, &lp_dir->data[ fd_array[ fd ].offset ], result);
      fd_array[fd].offset += result;        // advance our "current" ptr
   }
}

// check ownership of fd and the fd is valid within range
int FScanAccessFD( int fd, int owner ) {
  if( fd_array[fd].owner == owner) return 1;
  return 0;     // not good
}

// Search our (fixed size) table of file descriptors. returns fd_array[] index
// if an unused entry is found, else -1 if all in use. if avail, then all
// fields are initialized.
int FSallocFD(int owner){
  int i;

  for(i=0; i<FD_NUM; i++) {
    if( 0 == fd_array[i].owner ) {
    fd_array[i].owner = owner;
    fd_array[i].offset = 0;
    fd_array[i].item = 0; // NULL is (void *)0, spede/stdlib.h

    return i;
    }
  }
  return -1;  // no free file descriptors
}

dir_t *FSfindName(char *name){
  dir_t *starting;

  // assume every path relative to root directory. Eventually, the user
  // context will contain a "current working directory" and we can possibly
  // start our search there
  if(name[0] == '/'){
    starting = root_dir;

    while(name[0] == '/') name++;

    if(name[0] == 0) return root_dir; // client asked for "/"
  } else{
  // path is relative, so start off at CWD for this process
  // but we don't have env var CWD, so just use root as well
    starting = root_dir; // should be what env var CWD is
  }

  if(name[0] == 0) return 0;

  return FSfindNameSub(name, starting);
}

// go searching through a single dir for a name match. use MyStrcmp()
// for case-insensitive compare. use '/' to separate directory components
// if more after '/' and we matched a dir, recurse down there
// RETURN: ptr to dir entry if found, else 0
// once any dir matched, don't return name which dir was matched
dir_t *FSfindNameSub(char *name, dir_t *this_dir){
  dir_t *dir_p = this_dir;
  int len = MyStrlen(name);
  char *p;

  // if name is '.../...,' we decend into subdir
  if((p = strchr(name, '/')) != 0) len = p - name;  // p = to where / is (high mem)

  for(; dir_p->name; dir_p++) {
    //  if((unsigned int)dir_p->name > 0xdfffff) return 0; // tmp bug-fix patch
    if(1 == MyStrcmp(name, dir_p->name, len)){
      if(p && p[1] != 0){ // not ending with name, it's "name/..."
        // user is trying for a sub-dir. if there are more components, make sure this
        // is a dir. if name ends with "/" we don't check. thus "hello.html/" is legal
        while(*p == '/'){
          p++;                                // skipping trailing /'s in name
          if( '\0' == *p ) return dir_p;      // name "xxx/////" is actually legal
        }
        // altho name given is "xxx/yyy," xxx is not a directory
        if(dir_p->mode != MODE_DIR) return 0; // bug-fix patch for "cat h/in"
        name = p;
        return FSfindNameSub(name, (dir_t *)dir_p->data);
      }
      return dir_p;
    }
  }
  return 0; // no match found
}

// copy what dir_p points to (dir_t) to what attr_p points to (attr_t)
void FSdir2attr( dir_t *dir_p, attr_t *attr_p ) {
  attr_p->dev = current_pid;   // current_pid manages this i-node
  attr_p->inode = dir_p->inode;
  attr_p->mode = dir_p->mode;
  attr_p->nlink = ( A_ISDIR( attr_p->mode ) ) + 1;
  attr_p->size = dir_p->size;
  attr_p->data = dir_p->data;
}

void FScloseHandler(void) {
  int fd;
  fd = pcb[current_pid].TF_p->eax;

  if(FScanAccessFD(fd, current_pid))fd_array[fd].owner = 0;
    else  cons_printf("FScloseHandler: cannot close FD!\n");
}

void ForkHandler(char *bin_code, int *child_pid) {
	int i;

	for(i = 0; i < MEM_PAGE_NUM; i++){
		if(mem_page[i].owner == 0) break;
	}
	
	if (i == MEM_PAGE_NUM){
		cons_printf("Kernel Panic: no memory page available!\n");
		*child_pid = 0;
		return;
	}

	if (free_q.size == 0){ // this may occur for testing
		cons_printf("Kernel Panic: no PID available!\n");
		*child_pid = 0;  // no PID can be returned
		return;
	}
	*child_pid = DeQ(&free_q);     // get 'pid' from free_q
	MyBzero((char *)&pcb[(int)&child_pid], sizeof(pcb_t));
	pcb[(int)&child_pid].state = READY;
	pcb[(int)&child_pid].ppid = current_pid;
	MyBzero((char *)&mem_page[(int)&child_pid].addr, sizeof(MEM_PAGE_SIZE)); // clear memory page
	mem_page[(int)&child_pid].owner = pcb[(int)&child_pid].ppid;
	MyMemcpy((char *)&mem_page[(int)&child_pid], bin_code, MEM_PAGE_SIZE);

	pcb[(int)&child_pid].TF_p = (TF_t *)&mem_page[(int)&child_pid + (MEM_PAGE_SIZE - sizeof(TF_t))]; // set trapframe ptr in PCB to near the end of the memory page (leave TF space)
	// then fill out the eip of the TF
	pcb[(int)&child_pid].TF_p->eip = (unsigned int)&mem_page[(int)&child_pid]; //  set the EIP of trapframe to the start of the memory page
	pcb[(int)&child_pid].TF_p->eflags = EF_DEFAULT_VALUE | EF_INTR; // EFL will enable intr!
	pcb[(int)&child_pid].TF_p->cs = get_cs(); // duplicate from current CPU
	pcb[(int)&child_pid].TF_p->ds = get_ds(); // duplicate from current CPU
	pcb[(int)&child_pid].TF_p->es = get_es(); // duplicate from current CPU
	pcb[(int)&child_pid].TF_p->fs = get_fs(); // duplicate from current CPU
	pcb[(int)&child_pid].TF_p->gs = get_gs(); // duplicate from current CPU
}

void WaitHandler(int *exit_num_p) {
	int child_pid, page_index;
	
	for(page_index = 0; page_index<PROC_NUM; page_index++){
		if (pcb[page_index].state == ZOMBIE && pcb[page_index].ppid == current_pid){
			child_pid = (int)&exit_num_p;
		} else {
			pcb[page_index].state = WAIT;
			current_pid = 0;
			return;
		}
	}
	EnQ(child_pid, &free_q);
	pcb[child_pid].state = FREE;
	mem_page[child_pid].owner = 0;
	// loop thru pcb[] looking for ppid being running PID and state ZOMBIE
}

void ExitHandler(int exit_num){
	int ppid, *exit_num_p, page_index;
}