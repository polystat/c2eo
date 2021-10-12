#include <stdio.h>
#include <stdlib.h>

int f(int a, int b) {
  int c = a + b;
  return c;
}

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = f(a, a);
    printf("simpleReturn[%d] = %d \n", a, result);
}