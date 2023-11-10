#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

#define MAX_TEXT_SIZE 256

char *noFspaces;
char buffer[MAX_TEXT_SIZE];

int main(void) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_TEXT_SIZE;
    mqd_t q;
    q = mq_open("/my_queue", O_CREAT | O_RDWR, 0666, &attr);
    if (q == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if (pid == 0) { // Child process
        mqd_t q_child = mq_open("/my_queue", O_WRONLY);
        FILE *fp = fopen("test.txt", "r");
        if (fp == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        while (fgets(buffer, MAX_TEXT_SIZE, fp) != NULL) {
            if (mq_send(q_child, buffer, strlen(buffer) + 1, 0) == -1) {
                perror("mq_send");
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        if (mq_send(q_child, "end", strlen("end") + 1, 0) == -1) {
            perror("mq_send");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        fclose(fp);
        mq_close(q_child);
    } else { // Parent process
        mqd_t q_parent = mq_open("/my_queue", O_RDONLY);
        while (1) {
            ssize_t bytes_read = mq_receive(q_parent, buffer, MAX_TEXT_SIZE, NULL);
            if (bytes_read == -1) {
                perror("mq_receive");
                break;
            }
            buffer[bytes_read] = '\0';
            if (strcmp(buffer, "end") == 0) {
                break;
            }
            printf("Text: %s\n", buffer);
            
            noFspaces = strtok(buffer, " ");
            int ct = 0;

            while (noFspaces != NULL)
            {
                ct++;
                noFspaces = strtok(NULL, " ");
            }

            printf("Number of words in file: %d\n", ct);
        }
        mq_close(q_parent);
        mq_unlink("/my_queue");


    }
    return 0;
}