// pipe.c file

#define PSIZE 8
#define BROKENPIPE 11

typedef struct pipe{
  char buf[PSIZE];
  int head, tail, data, room;
  int status;
  int nreader, nwriter;    // number of reader, writer on this pipe
}PIPE;


#define NPIPE  4
PIPE pipe[NPIPE];

int pipe_init()
{
  // fill in code
  int i;
  for (i = 0; i < NPIPE; i++) {
    pipe[i].head = 0;
    pipe[i].tail = 0;
    pipe[i].data = 0;
    pipe[i].room = PSIZE;
    pipe[i].status = FREE;
    pipe[i].nreader = 0;
    pipe[i].nwriter = 0;
  }

}

PIPE *create_pipe()
{
  // fill in code
  int i;
  for (i = 0; i < NPIPE; i++) {
    if (pipe[i].status == FREE) {
      pipe->status = BUSY;
      pipe->nreader = pipe->nwriter = 1;
      return &pipe[i];
    }
  }
  return 0;
}

int read_pipe(PIPE *p, char *buf, int n)
{
  // fill in code;
  // add code to handle: no data AND no writer: return 0
  int r = 0;
  if (n <= 0) {
    return 0;
  }
  if (p->status == FREE) {
    return 0;
  }
  if (p->data == 0 && p->nwriter == 0) {
    return 0;
  }

  while (n) {
    while (p->data) {
      *buf++ = p->buf[p->tail++];
      p->tail %= PSIZE;
      p->data--; p->room++; r++; n--;
      if (n == 0) {
        break;
      }
    }
    wakeup(&p->room);
    if (r) {
      return r;
    }
    sleep(&p->data);
  }
  
}

int write_pipe(PIPE *p, char *buf, int n)
{
  // fill in code;
  // add code to detect BROKEN pipe: print BROKEN pipe message, then exit
  int r = 0;
  if (n <= 0) {
    return 0;
  }
  if (p->status == FREE) {
    return 0;
  }
  if (p->nreader == 0) {
    printf("BOKEN pipe condition met.\n");
    kexit(BROKENPIPE);
  }

  while(n) {
    while(p->room) {
      p->buf[p->head++] = *buf++;
      p->head %= PSIZE;
      p->data++; p->room--; r++; n--;
      if (n==0) {
        break;
      }
    }

    wakeup(&p->data);
    if (n == 0) {
      return r;
    }
    sleep(&p->room);
  }

}


  
