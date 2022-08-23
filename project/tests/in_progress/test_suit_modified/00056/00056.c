#include <stdio.h>

int test()
{
   int a;
   a = 42;
   printf("%d\n", a);

   int b = 64;
   printf("%d\n", b);

   int c = 12, d = 34;
   printf("%d\n", c);
   printf("%d\n", d);

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
