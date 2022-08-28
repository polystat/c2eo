#include <stdio.h>

#define NULL 0

int test()
{
   int a;
   int *b;
   int *c;

   a = 42;
   b = &a;
   c = NULL;

   printf("%d\n", *b);

   if (b == NULL)
      printf("b is NULL\n");
   else
      printf("b is not NULL\n");

   if (c == NULL)
      printf("c is NULL\n");
   else
      printf("c is not NULL\n");

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
