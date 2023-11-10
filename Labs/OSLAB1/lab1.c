#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (void){
    int fd[2];
    pipe(fd);

    int pid = fork();
    if(pid == 0) {
        close(fd[0]);  //Close read end
        dup2(fd[1],1); //duplicate (old fd, new fd) 1 = std_out
        close(fd[1]);  //Close write end
        execlp("ls", "ls", "/", NULL);
        printf("this will only happen if exec fails\n");
    } else {
        close(fd[1]);    //Close write end
        dup2(fd[0], 0);  //duplicate (old fd, new fd) 0 = std_in
        close(fd[0]);    //Close read end
        wait(NULL);
        execlp("wc", "wc", "-l", NULL);
    }
    return 0;
}
