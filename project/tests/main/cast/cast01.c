//clang -Xclang -ast-dump -fsyntax-only test07.c

#include <stdio.h>

double x = 4.5;
main() {
    printf("%e\n", x);
    x;
    long long y1, y2;
	y1 = x;
    printf("%d\n", y1);
    y1;
	y2 = (double) x;
    printf("%d\n", y2);
    y2;
}
