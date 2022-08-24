#include <stdio.h>

int fred()
{
//    printf("fred\n");
   return 0;
}

int joe()
{
//    printf("joe\n");
   return 1;
}

int test()
{
   int x;
   x = fred() && joe();
   printf("%d\n", x);
   x = fred() || joe();
   printf("%d\n", x);
   x = joe() && fred();
   printf("%d\n", x);
   x = joe() || fred();
   printf("%d\n", x);
   x = fred() && (1 + joe());
   printf("%d\n", x);
   x = fred() || (0 + joe());
   printf("%d\n", x);
   x = joe() && (0 + fred());
   printf("%d\n", x);
   x = joe() || (1 + fred());
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
