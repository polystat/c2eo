#if 0 != (0 && (0/0))
   #error 0 != (0 && (0/0))
#endif

#if 1 != (-1 || (0/0))
   #error 1 != (-1 || (0/0))
#endif

#if 3 != (-1 ? 3 : (0/0))
   #error 3 != (-1 ? 3 : (0/0))
#endif

int
test()
{
	return 0;
}

#include <stdio.h>
int main()
{
   int x = test();
   printf("%d\n", x);
   return x;
}
