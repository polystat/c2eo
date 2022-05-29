#include <stdio.h>

int test()
{
   int a = 1;

   if (a)
      printf("a is true\n");
   else
      printf("a is false\n");

   int b = 0;
   if (b)
      printf("b is true\n");
   else
      printf("b is false\n");

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}