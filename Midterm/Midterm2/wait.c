int tswitch();

PROC *remove(PROC** list, int event) {

}

int ksleep(int event)
{
  //printf("ksleep: under construction\n");
  running->status = SLEEP;
  //enqueue(&sleepList, running);
  running->event = event;
  tswitch();
}

int kwakeup(int event)
{
  //printf("kwakeup: under construction\n");

  PROC *p, *q;

  while (p)
  {
    if (p->event ==  event && p->status == SLEEP) {
      //q = remove(&sleepList, event);
      q->status = READY;
      enqueue(&readyQueue, q);
    }

  } 
}

int kexit(int exitValue)
{
  printf("proc %d in kexit(), value=%d\n", running->pid, exitValue);
  running->exitCode = exitValue;
  running->status = ZOMBIE;

  //give children to parent
  PROC *p = running->child;
  PROC *pp = running->parent;

  addchild(pp, p);
  running->child = 0;

  kwakeup(running->parent);
  tswitch();
}

int kwait() {
  PROC *p = running;
  int exitCode;
  while (1) {
    if (p->child == 0) {
      return -1;
    }
    else {
      p = p->child;
      while (p) {
        if (p->status == ZOMBIE) {
          //free zombie
          exitCode = p->exitCode;
          p->status = FREE;
          enqueue(&freeList, p);
          //remove from child list
          p = p->parent;

          return exitCode;
        } 
        p = p->sibling;
      }
    }
    ksleep(running);
  }
}
  
