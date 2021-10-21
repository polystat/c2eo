#include <stdio.h>

static int a = 10;

int foo() {
    a += 1;
    printf("a = %d\n", a);
    static int* pa = &a;
    static int a = 20;
    a += 3;
    printf("a = %d\n", a);
    {
        static int a = 30;
        a += 7;
        printf("a = %d\n", a);
        {
            static int a = 40;
            a += 15;
            printf("a = %d\n", a);
            {
                static int a = 50;
                a += 31;
                printf("a = %d\n", a);
            }
        }
    }
    printf("a = %d\n", *pa);
}

int main() {
    foo();
    foo();
}