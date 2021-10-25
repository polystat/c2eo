// doubleVarDecl.c
//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>

double d01;
double d02 = 3.0 + 5.0;
double d03 = 3.14;      // не конвертирует
static double d04 = 3.14;

int main() {
    printf("%e\n", d01);
    d01;
    printf("%e\n", d02);
    d02;
    printf("%e\n", d03);
    d03;
    printf("%e\n", d04);
    d04;
}
