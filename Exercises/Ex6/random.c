//
// Created by adrian on 2023-11-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define HIGH 20
#define FREQ 80

void init(int *sequence, int refs, int pages) {
    int high = (int) (pages * ((float) HIGH / 100));

    for (int i = 0; i < refs; i++){
        if(rand() % 100 < FREQ)
    sequence[i] = rand() % high;
    else 
    sequence[i] = high + rand() % (pages - high);
}
}
int main(int argc, char *argv[]) {
/* could be command line arguments */
    int refs = 10;
    int pages = 100;
    int *sequence = (int *) malloc(refs * sizeof(int));
    init(sequence, refs, pages);
/* a small experiment to show that it works */
    for (int i = 1; i < refs; i++)
        printf(", %d", sequence[i]);
    printf("\n");
    return 0;
}
