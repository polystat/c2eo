#include <stdio.h>
typedef long I;
typedef struct{I c[4];I b,e,k;} PT;

PT cases[] = {1,2,3,4,5,6,7};

int main() {
	printf("%ld %ld %ld %ld %ld %ld %ld\n", cases[0].c[0], cases[0].c[1],
         cases[0].c[2], cases[0].c[3], cases[0].b, cases[0].e, cases[0].k);
    return 0;
}
