#include "stdio.h"
int a=13;
int b=0;
int main() {
    b+=(a+=1);
    b;
    printf("%d\n", b);
    return 0;
}