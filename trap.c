#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
struct new_table
{
  struct spinlock lock;
  struct proc proc[NPROC];
};
extern struct new_table ptable;
// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[]; // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void tvinit(void)
{
  int i;

  for (i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE << 3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void trap(struct trapframe *tf)
{
  if (tf->trapno == T_SYSCALL)
  {
    if (myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if (myproc()->killed)
      exit();
    return;
  }

  switch (tf->trapno)
  {
  case T_IRQ0 + IRQ_TIMER:
    if (cpuid() == 0)
    {
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
      // struct proc *p;
      for (struct proc *p = ptable.proc; p < &ptable.proc[NPROC]; p++)
      {
        // if(p->pid==4)
        // cprintf("p->state = %d\n",p->state);
        if (p->state == RUNNING)
          p->rtime += 1;
      }
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE + 1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;

  //PAGEBREAK: 13
  default:
    if (myproc() == 0 || (tf->cs & 3) == 0)
    {
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running
  // until it gets to the regular system call return.)
  if (myproc() && myproc()->killed && (tf->cs & 3) == DPL_USER)
    exit();

#ifdef MLFQ
  struct proc *p1; //=myproc();
  for (p1 = ptable.proc; p1 < &ptable.proc[NPROC]; p1++)
  {
    if (p1->state == RUNNING)
    {
      if (p1->quantums_left != 0)
      {
        p1->quantums_left--;
        p1->ticks[p1->queue]++;
        // if (p1->pid == 5 || p1->pid ==4)
        //   cprintf("p1->pid =%d  p1->ticks[%d] = %d\n", p1->pid,p1->queue, p1->ticks[p1->queue]);
        // p1->
      }

      else if (p1->quantums_left == 0)
      {
        // cprintf("before assignong quantums , pid=%d quantums_left=%d\n", p1->pid, p1->quantums_left);
        if (p1->queue != 4)
        {
          p1->quantums_left = 1 << (++p1->queue);
        }
        else
        {
          p1->quantums_left = 16;
        }
        // cprintf("after assignong quantums ,pid=%d quantums_left=%d\n", p1->pid, p1->quantums_left);
        yield();
      }
    }
  }
#endif
#if defined(PBS) || defined(ROBIN)
  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if (myproc() && myproc()->state == RUNNING &&
      tf->trapno == T_IRQ0 + IRQ_TIMER)
    yield();

#endif
  // Check if the process has been killed since we yielded
  if (myproc() && myproc()->killed && (tf->cs & 3) == DPL_USER)
    exit();
}
