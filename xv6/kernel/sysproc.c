#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
/* The following code is added by Wesley Baskett | wlb210002 */
#include "pstat.h"
/* End of code added */

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
/* The following code is added by Wesley Baskett | wlb210002
 * System calls to set tickets for the current process and to get statistics
 * for all current processes in ptable
 * */

int sys_settickets(void){
  int n; // int to put the the amount of tickets passed
  
  //if we fail to get an int parameter, return -1 
  if (argint(0, &n) < 0)
 	return -1;

  //if the the passed ticket num is less than 1, syscall failed 
  if (n < 1)
  	return -1;
  //Set the current procs tickets
  proc->tickets = n;
  return 0;
}

int sys_getpinfo(void){
  char *addr;
  struct pstat *p;

  // If we fail to find the pointer passed as a parameter the syscall fails
  if (argptr(0, &addr, sizeof(*p)) < 0)
    return -1;
  // If the pointer is set to 0x0 (NULL), reject
  if (addr == 0)
    return -1;
  return getpinfo((struct pstat *)addr);
}

/* End of code added */

/* The following code is added by Robert Reece | rwr230001 */
/* This code implments the random syscall to generate random numbers */
/* function uses the xorshift algorithm */
/* return an integer from 0 to r-1 */
int sys_rng(void)
{
  int seed = sys_uptime();
  int range;
  if(argint(0,&range) < 0){return -1;} // get range from args return an error if none given
  if(range <= 0){return -1;} //return an error if given an invalid range

  //constants used for xor shift from wikipedia xorshift article
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed % range;
}

/* End of code added */
