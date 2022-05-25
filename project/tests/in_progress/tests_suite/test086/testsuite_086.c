#include <stdio.h>

int test()
{
   int a;
   int b;
   int *d;
   int *e;
   d = &a;
   e = &b;
   a = 12;
   b = 34;
   printf("%d\n", *d);
   printf("%d\n", *e);
   printf("%d\n", d == e);
   printf("%d\n", d != e);
   d = e;
   printf("%d\n", d == e);
   printf("%d\n", d != e);

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}