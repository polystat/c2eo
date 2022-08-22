#include <stdio.h>
#include <string.h>

int test()
{
   char a[10];
   strcpy(a, "abcdef");
   printf("%s\n", &a[1]);

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
