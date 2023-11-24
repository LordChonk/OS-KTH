//
// Created by adrian on 2023-11-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define SAMPLES 20
#define HIGH 20
#define FREQ 80

typedef struct pte {
    int id;
    int present;
    int refd;
    struct pte *next;
} pte;

void init(int *sequence, int refs, int pages) {
    int high = (int) (pages * ((float) HIGH / 100));
    if (high > 2) high = 2;

    int prev = pages;

    for (int i = 0; i < refs; i++) {
        if (rand() % 100 < FREQ) {
            int rnd;
            do {
                rnd = rand()%high;
            }while(rnd == prev);
            prev = rnd;
            sequence[i] = rnd;
        }else{
                int rnd;
                do{
                    rnd = high + rand() % (pages-high);
                }while (rnd == prev);
                prev = rnd;
                sequence[i]= rnd;
        }
    }
}

void clear_page_table(pte *page_table, int pages) {
    for (int i = 0; i < pages; i++) {
        page_table[i].id = i;
        page_table[i].present = 0;
        page_table[i].refd = 0;
        page_table[i].next = NULL;

    }
}

int simulate(int *seq, pte *table, int refs, int frames, int pages) {
    int hits = 0;
    int allocated = 0;

    pte *last = NULL;

    for (int i = 0; i < refs; i++) {
        int next = seq[i];
        pte *entry = &table[next];

        if (entry->present == 1) {
            entry->refd;
            hits++;
        } else {
            if (allocated < frames) {
                allocated++;
                entry->present = 1;

                if (last != NULL) {
                    entry->next = last ->next;
                    last -> next = entry;
                    last = entry;
                }else{
                        last = entry;
                        entry->next = entry;
                }
            }else{
             pte *cand = last->next;

             while(cand -> refd !=0){
                 cand ->refd = 0;
                 last = cand;
                 cand = cand->next;
             }

             cand -> present = 0;
             cand -> refd = 0;

             entry->present = 1;
             entry -> refd = 0;
             entry->next = cand -> next;
             last->next = entry;
                last = entry;
            }

        }
    }
    return hits;
}

int main(int argc, char *argv[]) {
    int refs = 10000000; //higher number gives better results
    int pages = 1000;

    int *sequence = (int *) malloc(refs * sizeof(int));
    init(sequence, refs, pages);
    pte *table = (pte *) malloc(pages * sizeof(pte));

    printf("# This is a benchmark of clock replacement\n");
    printf("# %d page references\n", refs);
    printf("# %d pages \n", pages);
    printf("#\n#\n#frames\tratio\n");
/* frames is the size of the memory in frames */
    int frames;
    int incr = pages / SAMPLES;

    for (frames = incr; frames <= pages; frames += incr) {
        /* clear page tables entries */
        clear_page_table(table, pages);
        int hits = simulate(sequence, table, refs, frames, pages);
        float ratio = (float) hits / refs;
        printf("%d\t%.2f\n", frames, ratio);
    }
    return 0;
}





