#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

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
 * TO BE IMPLEMENTED! DOES NOT WORK
 * */

int settickets(void){
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

int getpinfo(void){
  struct pstat pst;

  // If we fail to find the pointer passed as a parameter the syscall fails
  if (argptr(0, &pst, sizeof(*pst)) < 0)
    return -1;
  //Loop over the process table, for every slot that is in use, grab the process and put it into pstat
  for(int i = 0; i < NPROC ; i++){
    if(ptable.proc[i].state != UNUSED){
      pst->inuse[i] = 1;
      pst->tickets[i] = ptable.proc[i].tickets;
      pst->pid[i] = ptable.proc[i].pid;
      pst->ticks[i] = ptable.proc[i].ticks;
    }else{
      pst->inuse[i] = 0;
      pst->tickets[i] = 0;
      pst->pid[i] = 0;
      pst->ticks[i] = 0;
  }

  return 0;
}
/* End of code added */
