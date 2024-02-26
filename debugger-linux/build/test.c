#include "stdio.h"
#include <stdlib.h>

void f1();
void f2();
void f();

int main(int argc, char *argv[]) {
    int a = 3;
    int b = 2;
    int c = a + b;
    a = 4;

    printf("1. a = %d!\n", a);
    printf("2. Ok, and c = %d!\n", c);

    f1();

    printf("Done!\n");
    return EXIT_SUCCESS;
}


void f1() {
    printf("Now we are in fun1\n");
    f2();
}

void f2() {
    printf("Now we are in fun2\n");
    f();
}

void a() {
    int foo = 1;
    printf("Now we are in fun-a\n");
}

void b() {
    int foo = 2;
    printf("Now we are in fun-b\n");
    a();
}

void c() {
    int foo = 3;
    printf("Now we are in fun-c\n");
    b();
}

void d() {
    int foo = 4;
    printf("Now we are in fun-d\n");
    c();
}

void e() {
    int foo = 5;
    printf("Now we are in fun-e\n");
    d();
}

void f() {
    int foo = 6;
    printf("Now we are in fun-f\n");
    e();
}
