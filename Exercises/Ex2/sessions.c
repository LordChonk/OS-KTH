//
// Created by adrian on 2023-11-05.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int pid = fork();

    if(pid == 0){
        int child = getpid();
        printf("child: session %d\n", getsid(child));
    }else{
        int parent = getpid();
        printf("parent: sesison %d\n", getsid(parent));
    }
    return 0;
}
