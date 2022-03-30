#include <stdio.h>

long long a = 1;
long long b = 2;
long long c = 3;
long long x;

void swapABC(){
    x = a;
    a = b;
    b = c;
    c = x;
}

void printALL(){
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    a;
    b;
    c;
}

int main(){
    printALL();
    swapABC();
    printALL();
}
