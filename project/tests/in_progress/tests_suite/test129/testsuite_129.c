#include <stdio.h>

int test()
{
   int a;

   for (a = 0; a < 2; a++)
   {
      int b = a;
   }

   printf("it's all good\n");

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}