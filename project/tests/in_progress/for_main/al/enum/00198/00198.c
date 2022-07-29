#include <stdio.h>

enum fred { a, b, c };

int main()
{
   printf("%d\n", a);
   printf("%d\n", b);
   printf("%d\n", c);

   enum fred d;

   typedef enum { e, f, g } h;
   printf("%d\n", e);
   printf("%d\n", f);
   printf("%d\n", g);

   typedef enum { i, j, k } m;

   printf("%d\n", i);
   printf("%d\n", j);
   printf("%d\n", k);

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
