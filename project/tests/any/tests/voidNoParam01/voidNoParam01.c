#include <stdio.h>

int a = 1;
int b = 2;
int c = 3;
int x;

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
