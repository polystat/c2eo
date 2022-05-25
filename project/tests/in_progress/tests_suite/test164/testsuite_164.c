#include <stdio.h>

int test()
{
   char a;
   short b;

   printf("%d %d\n", sizeof(char), sizeof(a));
   printf("%d %d\n", sizeof(short), sizeof(b));

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}