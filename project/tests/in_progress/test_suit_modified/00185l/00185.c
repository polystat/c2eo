#include <stdio.h>

long test()
{
   long Count;

   long Array[10] = { 12, 34, 56, 78, 90, 123, 456, 789, 8642, 9753 };

   long tmpVal;
   for (Count = 0; Count < 10; Count++) {
      tmpVal = Array[Count];
      printf("%ld\n", Count);
      printf("%ld\n", tmpVal);
   }

   long Array2[10] = { 12, 34, 56, 78, 90, 123, 456, 789, 8642, 9753, };

   for (Count = 0; Count < 10; Count++) {
      tmpVal = Array2[Count];
      printf("%ld\n", Count);
      printf("%ld\n", tmpVal);
//       printf("%d: %d\n", Count, Array2[Count]);
   }

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
