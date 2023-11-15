//
//Created by adrian on 2023-11-15.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//init
volatile int ct = 0;

volatile int req[2] = {0,0};
volatile int turn = 0;

//lock and unlock
void lock(int id){
    req[id] = 1;
    int other = 1 - id;
    turn = other;
    while(req[other] == 1 && turn == other) {

    }; //spin
}

void unlock(int id){
    req[id] = 0;
}

//describe threads
typedef struct args {int inc; int id;} args;

void *increment(void *arg){
    int inc =((args*)arg)->inc;
    int id = ((args*)arg) ->id;
    printf("start: %d\n", id);

    for(int i = 0; i < inc; i++){
        lock(id);
        ct++;
        unlock(id);
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
