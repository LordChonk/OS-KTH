//
// Created by adrian on 2023-11-05.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
        int pid = fork();

        if(pid==0)
            return 42;
        else{
            int res;
            wait(&res);
            printf("The result was %d\n", WEXITSTATUS(res));
        }


        /* Part 1
        if(pid==0) {
            printf("I´m the child %d\n", getpid());
            sleep(1);
        }else {
            printf("My child is called %d\n", pid);
            wait(NULL);  //need: #include <sys/wait.h> btw
            printf("My child has terminated \n");
        }
        printf("This is the end(%d)\n", getpid());*/


        //printf("That´s it %d\n", getpid());

        //printf("pid = %d\n\n", pid);

        return 0;

}
