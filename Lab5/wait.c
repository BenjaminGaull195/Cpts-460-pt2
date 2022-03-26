int ksleep(int event)
{
  // your ksleep code
  //printf("ksleep: under construction\n");
  running->status = SLEEP;
  enqueue(&sleepList, running);
  running->event = event;
  tswitch();
}

int kwakeup(int event)
{
  // your kwakeup code

  PROC *p, *q;

  while ((p = dequeue(&sleepList)) != 0)
  {
    if (p->event ==  event && p->status == SLEEP) {
      //q = remove(&sleepList, event);
      p->status = READY;
      enqueue(&readyQueue, p);
    }
    else {
      enqueue(&q, p);
    }
  }   
  sleepList = q;
}

int kexit(int value)
{
  // your kexit code
  int i;
  PROC *p;

  printf("proc %d in kexit(), value=%d\n", running->pid, value);
  running->exitCode = value;
  running->status = ZOMBIE;

  //give children to parent
  for (i = 0; i < NPROC; i++) {
    p = &proc[i];
    if (p->ppid == running->pid) {
      p->ppid = running->ppid;
      p->parent = running->parent;
    }
  }

  kwakeup(running->parent);
  tswitch();

}

int kwait(int *status)
{
  // your kwait code
  int i;
  int cpid = -1;
  PROC *p = running;
  int exitCode;
  for (i = 0; i < NPROC; i++) {
    p = &proc[i];
    if (p->status == ZOMBIE) {
      *(status) = p->exitCode;
      p->status = FREE;
      enqueue(&freeList, p);
      return p->pid;
    }
  }

  if (cpid == -1) {
    return cpid;
  }

  ksleep(running);
}



