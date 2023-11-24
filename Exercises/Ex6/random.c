//
// Created by adrian on 2023-11-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
void init(int *sequence, int refs, int pages) {
    for (int i = 0; i<refs; i++)
        sequence[i] = rand() % pages;
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
