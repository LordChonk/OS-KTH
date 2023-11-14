//
// Created by adrian on 2023-11-14.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IO_TIME 30

int io_op(float ratio, int exect){
    int io =((float)rand()) / RAND_MAX<ratio;
    if(io)
        io = (int) trunc(((float)rand())/RAND_MAX * (exect - 1)) +1;
    return io;
}


typedef struct job{
    int id;
    int arrival;
    int unblock;
    int turnaround; //SJF
    int response;
    int exectime;
    float ioratio;
    struct job *next;
} job;

typedef struct spec {
    int arrival;
    int exectime;
    float   ioratio;
} spec;
//Create dummy job
spec specs[] = {
        { 0, 10, 0.0},
        { 0, 30, 0.7},
        { 0, 20, 0.0},
        { 40, 80, 0.4},
        { 60, 30, 0.3},
        {120, 90, 0.3},
        {120, 40, 0.5},
        {140, 20, 0.2},
        {160, 10, 0.3},
        {180, 20, 0.3},
        {0, 0, 0}
};

//Set up queues
job *readyq = NULL;
job *blockedq = NULL;
job *doneq = NULL;

//Ready queue
void ready(job *this){
    job *nxt = readyq;
    job *prev = NULL;

    while (nxt != NULL && nxt->exectime <=this->exectime){ //adding check for sjf
        prev = nxt;
        nxt = nxt -> next;
    }
    this -> next = nxt;
    if(prev == NULL)
        readyq = this;
    else
        prev -> next = this;

    return;
}

//Done queue
void done(job *this){
    this ->next =doneq;
    doneq = this;
    return;
}
//Block and unblock
void block(job *this) {
    job *nxt = blockedq;
    job *prev = NULL;

    while (nxt != NULL) {
        if (this->unblock < nxt->unblock)
            break;
        else {
            prev = nxt;
            nxt = nxt->next;
        }
    }
    this->next = nxt;
    if (prev != NULL)
        prev->next = this;
    else
        blockedq = this;
    return;
}

void unblock(int time) {
    while (blockedq != NULL && blockedq->unblock <= time) {
        job *nxt = blockedq;
        blockedq = nxt->next;
        printf("(%d)unblock job %2d\n", time, nxt->id);
        ready(nxt);
    }
}


//Scheduler
int schedule(int time, int slot) {
    if (readyq != NULL) {
        job *nxt = readyq;
        readyq = readyq->next;

        if (nxt->response == 0)
            nxt->response = time - nxt->arrival;

        int left = nxt->exectime;
        int io = 0;
        int exect = (left < slot) ? left : slot;

        //io
        if (exect > 1) {
            io = io_op(nxt->ioratio, exect);
            if (io)
                exect = io;
        }
            nxt->exectime -= exect;
            printf("(%4d)run job %2d for %3d ms ", time, nxt->id, exect);

            if (nxt->exectime == 0) {
                nxt->turnaround = time + exect - nxt->arrival;
                printf(" - done\n");
                done(nxt);
            } else {
                if (io) {
                    nxt-> unblock = time + exect + IO_TIME;
                    block(nxt);
                    printf(" - %3d left - blocked\n", nxt->exectime);
                    /*ready(nxt);
                    printf(" - %3d left - I/O \n", nxt->exectime);
                    exect += IO_TIME;*/
                }else {
                    ready(nxt);
                    printf(" - %3d left \n", nxt->exectime);
                }
            }
            return exect;
        }else{
        return 1;
    }
    }

//init
void init() {
    int i = 0;
    while (specs[i].exectime != 0) {
        job *new = (job *) malloc(sizeof(job));
        new->id = i + 1;
        new->arrival = specs[i].arrival;
        new->unblock = specs[i].arrival;
        new->exectime = specs[i].exectime;
        new->ioratio = specs[i].ioratio;
        block(new);
        i++;
    }
}

int main(int argc, char *argv[]) {
    int slot = 15;

    if (argc == 2) {
        slot = atoi(argv[1]);
    }
    init();
    int time = 0;

    while (blockedq != NULL || readyq != NULL) {
        unblock(time);
        int tick = schedule(time, slot);
        time += tick;
    }

    int turnaround = 0;
    int jobs = 0;

    for (job *nxt = doneq; nxt != NULL; nxt = nxt->next) {
        jobs += 1;
        turnaround += nxt->turnaround;
    }

    printf("\navaerage turnaround: %d \n", turnaround / jobs);

    printf("\ntotal execution time is %d \n", time);

    return 0;
}



