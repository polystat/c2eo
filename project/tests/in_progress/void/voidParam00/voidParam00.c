#include <stdio.h>

int a = 6;
int b = 8;

void fun(int x) {
    a = b * 2;
    a;
    b;
    printf("%d\n", a);
    printf("%d\n", b);
}

int main(){
    fun(5);
    fun(b);
}
