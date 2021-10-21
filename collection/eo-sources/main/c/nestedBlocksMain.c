#include <stdio.h>

static int a = 10;

int main() {
    static int* pa = &a;
    static int a = 20;
    printf("a = %d\n", a);
    {
        static int a = 30;
        printf("a = %d\n", a);
        {
            static int a = 40;
            printf("a = %d\n", a);
            {
                static int a = 50;
                printf("a = %d\n", a);
            }
        }
    }
    printf("a = %d\n", *pa);
}