#include <stdio.h>

int test()
{
   char a;
   int b;
   double c;

//    printf("%d\n", sizeof(a));
//    printf("%d\n", sizeof(b));
//    printf("%d\n", sizeof(c));
//    printf("%d\n", sizeof(!a));

   int x;
   x = sizeof(a);
   printf("%d\n", x);

   x = sizeof(b);
   printf("%d\n", x);

   x = sizeof(c);
   printf("%d\n", x);

//    x = sizeof(!a);
//    printf("%d\n", x);

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/

#include <stdio.h>
int main()
{
   int x = test();
   printf("%d\n", x);
   return x;
}
