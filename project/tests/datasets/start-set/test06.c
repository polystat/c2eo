//clang -Xclang -ast-dump -fsyntax-only test06.c

#include <stdio.h>

int a = 2;
double  pi = 3.0;
static double r = 10.0;
double p;
double s;
int main() {
    p = 2.0 * pi * r;
    printf("%e\n", p);
    p;
    s = pi * r * r;
//    printf("%e\n", s);
    s;
}
