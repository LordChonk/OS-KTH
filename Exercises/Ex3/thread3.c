//
// Created by adrian on 2023-11-07.
//
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct thread_args {
    int a; double b;
};

struct thread_result{
    long x; double y;
};

void *thread_func(void *argsVoid){
    struct thread_args *args = argsVoid;
    struct thread_result *res = malloc(sizeof *res);

    res ->x = 10 + args ->a;
    res ->y = args->a * args->b;
    return res;
}

int main(){
    pthread_t threadL;
    struct thread_args in = {.a = 10, .b = M_PI};
    void *out_void;
    struct thread_result *out;

    pthread_create(&threadL, NULL, thread_func, &in);
    pthread_join(threadL, &out_void);
    out = out_void;
    printf("out -> x = %d\t out -> b = %f\n", out ->x, out ->y);
    free(out);

    return 0;
}