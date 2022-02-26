extern PROC *running;
extern PROC *readyQueue;

int block(struct semaphore *s) {
  running->status = BLOCK;
  enqueue(&s->queue, running);
  tswitch();
}

int signal(struct semaphore *s) {
  PROC *p = dequeue(&s->queue);
  p->status = READY;
  enqueue(&readyQueue, p);
}

int P(struct semaphore *s)
{
  // fill in code
  int SR = int_off();
  if (s->value == 1) {
    s->value = 0;
  }
  else {
    block(s);
  }
  int_on(SR);
}

int V(struct semaphore *s)
{
  // fill in code
  int SR = int_off();
  if (s->queue == 0) {
    s->value = 1;
  }
  else {
    signal(s);
  }
  int_on(SR);
}

