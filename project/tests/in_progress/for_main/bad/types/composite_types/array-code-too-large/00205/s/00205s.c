#include <stdio.h>
typedef struct{long c[2];long b,e,k;} PT;

PT arr[] = {1,2,3,4,5};

int main() {
	printf("%ld %ld %ld %ld %ld %ld %ld\n", arr[0].c[0], arr[0].c[1],
         arr[0].b, arr[0].e, arr[0].k);
        printf("%ld %ld %ld %ld %ld %ld %ld\n", &arr[0].c[0], &arr[0].c[1],
               &arr[0].b, &arr[0].e, &arr[0].k);
    return 0;
}
