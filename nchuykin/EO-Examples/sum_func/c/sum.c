#include <stdio.h>
#include <stdlib.h>


int sum(int a, int b)
{
   int res = a + b;
   return res;
}


int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    printf("%d + %d = " , a, b);
    int res = sum(a, b);
    printf("%d\n", res);
}
