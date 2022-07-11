
#include <stdio.h>

long long x = 3;
main() {
    printf("%d\n", x);
    double y1, y2;
	y1 = x;
    printf("%e\n", y1);
	y2 = (long long) x;
    printf("%e\n", y2);
}
