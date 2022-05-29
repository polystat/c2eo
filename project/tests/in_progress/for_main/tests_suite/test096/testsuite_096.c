#include <stdio.h>

void fred(void)
{
   printf("yo\n");
}

int test()
{
   fred();

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}