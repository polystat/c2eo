#include <stdio.h>

int test()
{
   int a;
   char b;

   a = 0;
   while (a < 2)
   {
      printf("%d", a++);
      break;

      b = 'A';
      while (b < 'C')
      {
         printf("%c", b++);
      }
      printf("e");
   }
   printf("\n");

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}