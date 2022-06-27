#include <stdio.h>

int test()
{
   char a;
   short b;

//    printf("%d %d\n", sizeof(char), sizeof(a));
//    printf("%d %d\n", sizeof(short), sizeof(b));

   int x;
   x = sizeof(char);
   printf("%d\n", x);
   x = sizeof(a);
   printf("%d\n", x);
   x = sizeof(short);
   printf("%d\n", x);
   x = sizeof(b);
   printf("%d\n", x);

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
