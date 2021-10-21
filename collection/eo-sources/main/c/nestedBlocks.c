#include <stdio.h>

static int a = 10;

int main() {
    int* pa = &a;
    int a = 20;
    printf("a = %d\n", a);
    {
        int a = 30;
        printf("a = %d\n", a);
        {
            int a = 40;
            printf("a = %d\n", a);
            {
                int a = 50;
                printf("a = %d\n", a);
            }
        }
    }

    printf("a = %d\n", *pa);
}