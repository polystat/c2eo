#include <stdio.h>

void fred(int x)
{
   switch (x)
   {
      case 1: printf("1\n"); return;
      case 2: printf("2\n"); break;
      case 3: printf("3\n"); return;
   }

   printf("out\n");
}

int test()
{
   fred(1);
   fred(2);
   fred(3);

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