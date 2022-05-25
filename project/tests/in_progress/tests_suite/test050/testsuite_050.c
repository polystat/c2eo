#include <stdio.h>
int printf(const char *, ...);
char t[] = "012345678";

int test(void)
{
    char *data = t;
    unsigned long long r = 4;
    unsigned a = 5;
    unsigned long long b = 12;

    *(unsigned*)(data + r) += a - b;

    printf("data = \"%s\"\n", data);
    return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}