#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_VAL 4

typedef struct{
        int VAR;
        int rdct;
        sem_t rdLock;
        sem_t wLock;
        sem_t order;
} shMem;

//Memory init
shMem *create_shared_memory(){
    shMem *shared = mmap(NULL, sizeof(shMem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //ANONLYMOUS flag so we avoid writing to a file
if(shared == MAP_FAILED){
    perror("mmap");
    exit(1);
}
return shared;
}
//Writer
void writer(shMem *shared){
    sem_wait(&shared->order);
    sleep(1);
    printf("The writer acquires the lock.\n");
        sem_wait(&shared->wLock);

        printf("The writer (%d) writes the value %d\n", getpid(), shared->VAR + 1);
        shared->VAR++;

        sem_post(&shared->wLock); //we can write again woohoo
    printf("The writer releases the lock.\n");
    sem_post(&shared->order);
        sleep(1);
    }


//Reader
void reader(shMem *shared){
        sem_wait(&shared->rdLock);
        shared->rdct++;
        if(shared->rdct == 1){
            sem_wait(&shared->wLock);
            printf("The first reader acquires the lock.\n");
        }
        sem_post(&shared->rdLock);

        printf("The reader (%d) reads the value %d\n", getpid(), shared->VAR);

        sem_wait(&shared->rdLock);
        shared->rdct--;
        if(shared->rdct == 0){
            printf("The last reader releases the lock.\n");
            sem_post(&shared->wLock); // unlcok write mutex
        }
        sem_post(&shared->rdLock);
        sleep(1);
    }


int main(){
    //shared memory using mmap
shMem *shared = create_shared_memory();
//Init
shared->VAR = 0;
shared->rdct = 0;
    sem_init(&shared->rdLock, 1, 1);
    sem_init(&shared->wLock, 1, 1);
    sem_init(&shared->order, 1, 0);

    //fork
    pid_t  pid1 = fork();
    if(pid1 == 0){
        reader(shared);
        sem_post(&shared->order);
        exit(0);
    }
    waitpid(pid1, NULL, 0);

    writer(shared);
    sem_post(&shared->order);

    pid_t pid2 = fork();
    if (pid2 == 0){
        sem_wait(&shared->order);
        reader(shared);
        exit(0);
    }

    pid_t pid3 = fork();
    if(pid3 == 0){
        sem_wait(&shared->order);
        reader(shared);
        exit(0);
    }

    writer(shared);

    //wait for child process to finish
    wait(NULL);
    wait(NULL);

    //Destroy semaphores and memory unmap
    sem_destroy(&shared->rdLock);
    sem_destroy(&shared->wLock);
    sem_destroy(&shared->order);
    munmap(shared, sizeof(shMem));

    return 0;

}