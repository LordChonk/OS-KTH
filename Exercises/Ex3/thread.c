//
// Created by adrian on 2023-11-07.
//
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *threadfunciton(void *arg){
    printf("%s\n", (char *)arg);
    return 0;
}

int main(){
    pthread_t thread;
    char *msg = "Hello world but with threads :)";

    int createerror = pthread_create(&thread, NULL, threadfunciton,msg);

    pthread_join(thread, NULL);
    return 0;
}
