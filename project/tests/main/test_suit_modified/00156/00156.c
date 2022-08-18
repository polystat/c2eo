#include <stdio.h>

int test()
{
   int Count;

   for (Count = 1; Count <= 10; Count++)
   {
      printf("%d\n", Count);
   }

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :

#include <stdio.h>
int main()
{
   int x = test();
   printf("%d\n", x);
   return x;
}
