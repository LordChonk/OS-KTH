#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//const int read_only =123456;

//char global [] = "This is a global string";
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

    int pid = getpid();
    unsigned long p = 0x1;

    foo(&p);

    back:
    printf(" p: %p \n", &p);
    printf(" back: %p \n", &&back);



    //printf("p: (0x%1x): %p \n", p, &p);
    /*foo:
    printf("process id: %d\n", pid);
    printf("global string: %p\n", &global);
    printf("read only: %p\n", &read_only);
    printf("the code: %o\n", &&foo);*/

    printf("\n\n /proc/%d/maps \n\n", pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    //fgetc(stdin);
    return 0;
}


