#include <stdio.h>
#include <stdlib.h>

int a;
int b;
int res;

void sum()
{
  res = a + b;
}


int main(int argc, char** argv) {
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    printf("%d + %d = " , a, b);
    sum();
    printf("%d\n", res);
}
