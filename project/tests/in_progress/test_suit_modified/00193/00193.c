#include <stdio.h>

void fred(int x)
{
   switch (x)
   {
      case 1: printf("%d\n", x); return;
      case 2: printf("%d\n", x); break;
      case 3: printf("%d\n", x); return;
   }

}

int test()
{
   fred(1);
   fred(2);
   fred(3);

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
