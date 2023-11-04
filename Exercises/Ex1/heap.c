//
// Created by adrian on 2023-11-04.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


void zot(unsigned long *stop){
    unsigned long r = 0x3;
    unsigned long *i;

    for(i = &r; i<= stop; i++)
        printf("%p 0x%1x\n", i, *i);
}


void foo(unsigned long *stop){
    unsigned long q =0x2;

    zot(stop);
}


int main() {
    long *heap = (unsigned long*) calloc(40,
     sizeof (unsigned long));
    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);

    free(heap);
    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);

    int pid = getpid();
    unsigned long p = 0x1;

    /*foo(&p);

    back:
    printf(" p: %p \n", &p);
    printf(" back: %p \n", &&back);*/

    /*char *heap = malloc(20);
    *heap =0x61;
    printf("heap pointing to:0x%x\n", *heap);
    free(heap);
    //free(heap); - baited core dump

    char *foo = malloc(20);
    *foo =0x62;
    printf("foo pointint to: 0x%x\n", *foo);

    //danger
    *heap = 0x63;
    printf("or is it pointing to: 0x%x\n", *foo);*/

    printf("the heap variable at: %p\n", &heap);
    printf("pointing to: %p\n", heap);

    printf("\n\n /proc/%d/maps \n\n", pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    return 0;
}
