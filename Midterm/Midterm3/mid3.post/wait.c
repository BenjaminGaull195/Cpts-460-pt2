// wait.c file: ksleep(), kwakeup(), kexit()

extern PROC *running;
extern PROC *readyQueue;
extern PROC *sleepList;

int ksleep(int event)
{
  running->event = event;
  running->status = SLEEP;
  running->next = sleepList;
  sleepList = running;
  tswitch();
}

int kwakeup(int event)
{
  int i;
  PROC *p, *sp;
  for (i = 0; i < NPROC; i++) {
    p = &proc[i];
    if (p->event == event && p->status == SLEEP) {
      p->status = READY;
      enqueue(&readyQueue, p);
    }
  }

  //remove from sleep list
  while ((p = dequeue(&sleepList) != 0)) {
    if (p->status == SLEEP) {
      enqueue(&sp, p);
    }
  }
}

int kexit(int exitCode)
{
  running->exitCode = exitCode;
  running->status = ZOMBIE;
  running->priority = 0;
  kwakeup(running->parent);
  tswitch();
}
