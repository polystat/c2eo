#include <stdio.h>

int test()
{
   int a;
   int p;
   int t;

   a = 1;
   p = 0;
   t = 0;

   while (a < 100)
   {
      printf("%d\n", a);
      t = a;
      a = t + p;
      p = t;
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
