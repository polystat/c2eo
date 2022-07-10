#include <stdio.h>

enum fred { a, b, c };

int main()
{
//    printf("a=%d\n", a);
//    printf("b=%d\n", b);
//    printf("c=%d\n", c);
   printf("%d\n", a);
   printf("%d\n", b);
   printf("%d\n", c);

   enum fred d;

   typedef enum { e, f, g } h;
   typedef enum { i, j, k } m;

//    printf("e=%d\n", e);
//    printf("f=%d\n", f);
//    printf("g=%d\n", g);
   printf("%d\n", e);
   printf("%d\n", f);
   printf("%d\n", g);

//    printf("i=%d\n", i);
//    printf("j=%d\n", j);
//    printf("k=%d\n", k);
   printf("%d\n", i);
   printf("%d\n", j);
   printf("%d\n", k);

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
