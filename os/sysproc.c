#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_numvp(void) {
  int i=0;
  int counter = 0;
  for (;i<myproc()->sz;i+=PGSIZE) {
    counter++;
  }
  return counter;
}

int sys_numpp(void) {
  return phys();
}

int sys_mmap(void) {
  int bytes = 0;
  argint(0,&bytes);
  if (bytes < 0 || bytes%PGSIZE != 0) {
    return 0;
  }
  myproc()->allocLeft += bytes;
  // cprintf("allocleft is %d,pid is %d\n",myproc()->allocLeft, myproc()->pid);
  int j = myproc()->sz;

  //create mappings for sz to sz + bytes... PTE_P = 0 and PTE_M = 1
  // mapKarDe(myproc()->sz,myproc()->sz + bytes);

  myproc()->sz += bytes;
  return j;
}
