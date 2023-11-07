//
// Created by adrian on 2023-11-07.
//
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *thread_func(void *arg){
    printf("I am thread #%d\n", *(int *)arg);
    return NULL;
}
/* Extra stuff
  void *thread_func(void *arg){
    int code = 5;
    return (void *)code;
}

 void *thread_func(void *arg){
    char buffer[64] = "Hello world!";
    return buffer;
}

 void *thread_func(void *arg){
    char *buffer = (char *)malloc(64);
    buffer = "Hello world!";
    return buffer;
*/


int main() {
    pthread_t t1, t2;
    int i, j;
    i = 1;
    j = 2;

    pthread_create(&t1, NULL, &thread_func, &i);
    pthread_create(&t2, NULL, &thread_func, &j);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("In main thread\n");
    return 0;
}
