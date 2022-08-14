#include <stdio.h>

int test()
{
   int a;
   char b;

   a = 0;
   while (a < 2)
   {
      a++;
      printf("%d\n", a);
//       printf("%d\n", a++);
      break;

      b = 'A';
      while (b < 'C')
      {
         printf("%c\n", b++);
      }
//       printf("e");
   }
   printf("%d\n", a);

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
