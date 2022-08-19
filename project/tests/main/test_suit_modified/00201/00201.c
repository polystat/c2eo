#include <stdio.h>	// printf()

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define AB(x) CAT(x,y)

int main(void)
{
  int xy = 42;
  int t;
  t = CAT(A,B)(x);
  printf("%d\n", t);
  return 0;
}
