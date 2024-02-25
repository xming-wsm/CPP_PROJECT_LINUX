#include "stdio.h"
#include <stdlib.h>
int main(int argc, char *argv[]) {
    int a = 3;
    int b = 2;
    int c = a + b;
    a = 4;

    printf("1. a = %d!\n", a);
    printf("2. Ok, and c = %d!\n", c);

    return EXIT_SUCCESS;
}
