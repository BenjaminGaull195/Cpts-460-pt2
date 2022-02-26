// timer.c file
/***** timer confiuguration values *****/
#define CTL_ENABLE          ( 0x00000080 )
#define CTL_MODE            ( 0x00000040 )
#define CTL_INTR            ( 0x00000020 )
#define CTL_PRESCALE_1      ( 0x00000008 )
#define CTL_PRESCALE_2      ( 0x00000004 )
#define CTL_CTRLEN          ( 0x00000002 )
#define CTL_ONESHOT         ( 0x00000001 )

// timer register offsets from base address
/**** byte offsets *******
#define TLOAD   0x00
#define TVALUE  0x04
#define TCNTL   0x08
#define TINTCLR 0x0C
#define TRIS    0x10
#define TMIS    0x14
#define TBGLOAD 0x18
*************************/
/** u32 * offsets *****/
#define TLOAD   0x0
#define TVALUE  0x1
#define TCNTL   0x2
#define TINTCLR 0x3
#define TRIS    0x4
#define TMIS    0x5
#define TBGLOAD 0x6

typedef volatile struct timer{
  u32 *base;            // timer's base address; as u32 pointer
  int tick, hh, mm, ss; // per timer data area
  char clock[16]; 
}TIMER;

volatile TIMER timer[4];  // 4 timers; 2 timers per unit; at 0x00 and 0x20

typedef struct timerQueueElement {
  struct timerQueueElement *next;
  int timer;
  PROC *proc;
} TIMERQUEUEELEMENT;

TIMERQUEUEELEMENT freeElements[20];

TIMERQUEUEELEMENT *timerQueue;

TIMERQUEUEELEMENT *getFreeTimer() {
  int i;
  for (i = 0; i < 20; i++) {
    if (freeElements[i].timer == 0) {
      return &freeElements[i];
    }
  }
}

int timerenqueue(TIMERQUEUEELEMENT **queue, TIMERQUEUEELEMENT *p)
{
  TIMERQUEUEELEMENT *q  = *queue;
  if (q==0){
    *queue = p;
    p->next = 0;
    return;
  }
  if (p->timer < q->timer){
    p->next = *queue;
    q->timer -= p->timer;
    *queue = p;
    return;
  }
  while (q->next && p->timer >= q->next->timer){
    p->timer -= q->timer;
    q = q->next;
  }
  p->next = q->next;
  q->timer -= p->timer;
  q->next = p;
}

TIMERQUEUEELEMENT *timerdequeue(TIMERQUEUEELEMENT **queue)
{
  TIMERQUEUEELEMENT *p = *queue;
  if (p)
    *queue = p->next;
  return p;
}

int timerprintQ(TIMERQUEUEELEMENT *p)
{
  kprintf("readyQueue = ");
  while(p){
    kprintf("[%d%d]->", p->timer,p->proc->pid);
    p = p->next;
  }
  kprintf("NULL\n");
}

extern int kpchar(char, int, int);
extern int unkpchar(char, int, int);
extern int kputc(char);

int k;

void timer_init()
{
  int i;
  TIMER *tp;
  kputs("timer_init()\n");

  for (i=0; i<4; i++){
    tp = &timer[i];
    if (i==0) tp->base = (u32 *)0x101E2000; 
    if (i==1) tp->base = (u32 *)0x101E2020; 
    if (i==2) tp->base = (u32 *)0x101E3000; 
    if (i==3) tp->base = (u32 *)0x101E3020;

    *(tp->base+TLOAD) = 0x0;   // reset
    *(tp->base+TVALUE)= 0x0;
    *(tp->base+TRIS)  = 0x0;
    *(tp->base+TMIS)  = 0x0;
    *(tp->base+TCNTL) = 0x62; //011-0000=|En|Pe|IntE|-|scal=00|32-bit|0=wrap|
    *(tp->base+TBGLOAD) = 0xE0000/120;

    tp->tick = tp->hh = tp->mm = tp->ss = 0;
    strcpy((char *)tp->clock, "00:00:00");
  }
}

void timer_handler(int n){
  
    TIMER *t = &timer[n];
    int i;

    t->tick++;
    if (t->tick == 60) {
      putcursor();
    }
    else if (t->tick == 120){
      t->tick = 0;

      t->ss++;
      if (t->ss == 60) {
        t->ss = 0;
        t->mm++;
        if (t->mm == 60) {
          t->mm = 0;
          t->hh++;
        }
      }

      timerQueue->timer--;
      while (timerQueue != 0 && timerQueue->timer == 0)
      {
        TIMERQUEUEELEMENT *t;
        t = timerdequeue(&timerQueue);

        kwakeup(t->proc);
      }
      

      for (i=0; i<8; i++) {
        unkpchar(t->clock[i], n, 70+i);
      }

      t->clock[7]='0'+(t->ss%10); t->clock[6]='0'+(t->ss/10);
      t->clock[4]='0'+(t->mm%10); t->clock[3]='0'+(t->mm/10);
      t->clock[1]='0'+(t->hh%10); t->clock[0]='0'+(t->hh/10);

      timerprintQ(timerQueue);

      //kputs("timer interrupt\n");
      clrcursor();
    }
    
    color = n;
    for (i=0; i<8; i++) {
      kpchar(t->clock[i], n, 70+i);
    }

    timer_clearInterrupt(n);
}

void timer_start(int n) // timer_start(0), 1, etc.
{
  TIMER *tp = &timer[n];

  *(tp->base+TCNTL) |= 0x80;    // set enable bit 7
}

int timer_clearInterrupt(int n) // timer_start(0), 1, etc.
{
  TIMER *tp = &timer[n];
  *(tp->base+TINTCLR) = 0xFFFFFFFF;
}

void timer_stop(int n) // timer_start(0), 1, etc.
{
  TIMER *tp = &timer[n];
  *(tp->base+TCNTL) &= 0x7F;  // clear enable bit 7
}
