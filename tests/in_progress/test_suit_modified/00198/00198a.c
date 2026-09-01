#include <stdio.h>

typedef enum fred { a, b, c } x;

int main()
{
   printf("a=%d\n", a);
   printf("b=%d\n", b);
   printf("c=%d\n", c);

   enum fred d;

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
