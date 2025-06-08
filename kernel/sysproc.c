#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


#ifndef zwl
// 这是作者zwl的代码片段
uint64
sys_trace(void)
{
  int mask;
  if(argint(0, &mask) < 0) // 获取用户程序传入的数据
    return -1;

  // struct proc *p = myproc();
  myproc()->zwl_syscall_trace = mask; // 设置当前进程的系统调用跟踪掩码
  return 0;
}
#endif // zwl


#ifndef zwl
// 这是作者zwl的代码片段
uint64
sys_sysinfo(void)
{
  struct sysinfo info; //创建一个新的info结构体实例

  zwl_freebytes(&info.freemem); // 初始化info结构体
  zwl_nproc(&info.nproc); // 获取当前进程数量

  uint64 dsaddr; // 用于存储用户空间传入的地址
  if(argaddr(0, &dsaddr) < 0) // 获取用户程序传入的地址
      return -1; // 如果获取地址失败，返回-1

  // 将info结构体的内容复制到用户空间
  if(copyout(myproc()->pagetable, dsaddr, (char *)&info, sizeof(info)) < 0)
      return -1;

  return 0; // 成功返回0

}
#endif // zwl