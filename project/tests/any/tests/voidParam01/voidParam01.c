#include <stdio.h>

int a = 6;
int b = 8;
double x = 1.5;

void fun(int x, double d) {
//    d;
//    d = 0.5;
//    d;

    printf("%d\n", a);
    a;
    a = b * 5;
    printf("%d\n", a);
    a;
}

int main(){
    fun(5, x);
    fun(b, 2.5);
}

