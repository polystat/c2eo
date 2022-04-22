#include <stdio.h>

int test() 
{
   int a;
   a = 42;
   printf("%d\n", a);

   int b = 64;
   printf("%d\n", b);

   int c = 12, d = 34;
   printf("%d, %d\n", c, d);

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}