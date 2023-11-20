#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
int sharedBuffer = 0;

void *threadFunction(void *arg) {
    pthread_t t_id = pthread_self();
    int *localModCount = malloc(sizeof(int));
    *localModCount = 0;

    for (int i = 0; i < 5; i++) {
        // Lock the mutex before accessing the shared buffer
        pthread_mutex_lock(&mutex);
        //Critical section
        printf("Thread %lu, PID %ld, Buffer Value: %d\n", t_id, (long)getpid(),
               sharedBuffer);
        sharedBuffer++;

        pthread_mutex_unlock(&mutex);

        (*localModCount)++;
        sleep(1);
    }
    pthread_exit((void *)localModCount);
}
int main() {
    pthread_t threads[3];
    int thread_args[3];
    void *mod_counter[3];
    int i;
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (i = 0; i < 3; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, (void *)&thread_args[i]);
    }

    // Join threads
    for (i = 0; i < 3; i++) {
        if (pthread_join(threads[i], &mod_counter[i]) != 0) {
            perror("Failed to join thread");
            return 1;
        }
        printf("Thread %d modified the buffer %d times\n", i, *(int *)mod_counter[i]);
        free(mod_counter[i]);
    }

    return 0;
}
