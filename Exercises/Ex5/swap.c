//
// Created by adrian on 2023-11-15.
//
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>

//init
volatile int ct = 0;

volatile int global = 0;

int try(volatile int *mutex){
    return __sync_val_compare_and_swap(mutex, 0, 1);
}
//lock and unlock
void lock(volatile int *mutex){
    while(try(mutex) != 0);
    }; //spin

void unlock(volatile int *mutex){
    *mutex = 0;
}

//describe threads
typedef struct args {int inc; int id; volatile int *mutex;} args;

void *increment(void *arg){
    int inc =((args*)arg)->inc;
    int id = ((args*)arg) ->id;
    volatile int *mutex = ((args*)arg) -> mutex;
    printf("start: %d\n", id);

    for(int i = 0; i < inc; i++){
        lock(mutex);
        ct++;
        unlock(mutex);
    }
}


int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("usage peterson <inc>\n");
        exit(0);
    }

    int inc = atoi(argv[1]);

    pthread_t one_p, two_p;
    args one_args, two_args;

    one_args.inc = inc;
    two_args.inc = inc;

    one_args.id = 0;
    two_args.id = 1;

    pthread_create(&one_p, NULL, increment, &one_args);
    pthread_create(&two_p, NULL, increment, &two_args);
    pthread_join(one_p, NULL);
    pthread_join(two_p, NULL);

    printf("result is %d\n", ct);
    return 0;

}
