// doubleVarDecl.c
//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>

double d01;
double d02 = 3.0 + 5.0;
double d03 = 3.14;      // не конвертирует
static double d04 = 3.14;

int main() {
    printf("%f\n", d01);
    d01;
    printf("%f\n", d02);
    d02;
    printf("%f\n", d03);
    d03;
    printf("%f\n", d04);
    d04;
}
