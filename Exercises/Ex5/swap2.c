//
// Created by adrian on 2023-11-15.
//
#include <stdio.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

//init
volatile int ct;

volatile int mutex;


int futex_wait(volatile int *futexp){
    return syscall(SYS_futex, futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
}

void futex_wake(volatile int *futexp){
    syscall(SYS_futex, futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
}

int try(volatile int *lock){
    __sync_val_compare_and_swap(lock, 0, 1);
}


//lock and unlock
int lock(volatile int *lock){
    int susp = 0;
    while(try(lock) != 0){
        susp++;
        futex_wait(lock);
}
return susp;
}

void unlock(volatile int *lock){
    *lock = 0;
    futex_wait(lock);
}

//describe threads
typedef struct args {int inc; int id; volatile int *mutex;} args;

void *increment(void *arg){
    int inc =((args*)arg)->inc;
    int id = ((args*)arg) ->id;
    volatile int *mutex = ((args*)arg) -> mutex;
    int susp = 0;
    printf("start: %d\n", id);

    for(int i = 0; i < inc; i++){
        susp = susp + lock(mutex);
        ct++;
        unlock(mutex);
    }
    printf("done %d, suspended %d times\n", id, susp);
}


int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("usage futex <inc>\n");
        exit(0);
    }

    int inc = atoi(argv[1]);

    pthread_t one_p, two_p;
    args one_args, two_args;

    one_args.inc = inc;
    two_args.inc = inc;

    one_args.id = 1;
    two_args.id = 2;

    one_args.mutex = &mutex;
    two_args.mutex = &mutex;

    pthread_create(&one_p, NULL, increment, &one_args);
    pthread_create(&two_p, NULL, increment, &two_args);
    pthread_join(one_p, NULL);
    pthread_join(two_p, NULL);

    printf("result is %d\n", ct);
    return 0;

}

