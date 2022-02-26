/******** message.c file ************/

// READ Chapter 5.13.4.3; Use its code

#define NMBUF 10
struct semaphore nmbuf, mlock;

int menqueue(MBUF **queue, MBUF *p)
{
    MBUF *q  = *queue;
    if (q==0){
        *queue = p;
        p->next = 0;
        return;
    }
    if ((*queue)->pri < p->pri){
        p->next = *queue;
        *queue = p;
        return;
    }
    while (q->next && p->pri <= q->next->pri){
        q = q->next;
    }
    p->next = q->next;
    q->next = p;
}

MBUF *mdequeue(MBUF **queue)
{
    MBUF *p = *queue;
    if (p)
        *queue = p->next;
    return p;
}
    
MBUF mbuf[NMBUF], *mbufList;
		    
int msg_init()
{
    int i;
    MBUF * mp;
    mbufList = 0;

    for (i = 0; i < NMBUF; i++) {
        menqueue(&mbufList, &mbuf[i]);
    }
    nmbuf.value = NMBUF;
    mlock.value = 1;
}

MBUF *get_mbuf()
{
    P(&nmbuf);
    P(&mlock);
    MBUF *mp = mdequeue(&mbufList);
    V(&mlock);
    return mp;
}

int put_mbuf(MBUF *mp)
{
    P(&mlock);
    menqueue(&mbufList, mp);
    V(&mlock);
    V(&nmbuf);
}

int send(char *msg, int pid)
{
    if (pid < 0) {
        return 0;
    }
    PROC *p = &proc[pid];
    MBUF *mp = get_mbuf();
    mp->pid = running->pid;
    mp->pri = 1;
    strcpy(mp->contents, msg);
    P(&p->mlock);
    menqueue(&p->mqueue, mp);
    V(&p->mlock);
    V(&p->nmsg);
    return 0;
}

int recv(char *msg)
{
    P(&running->nmsg);
    P(&running->mlock);
    MBUF *mp = mdequeue(&running->mqueue);
    V(&running->mlock);
    strcpy(msg, mp->contents);
    int sender = mp->pid;
    put_mbuf(mp);
    return sender;
}


  
