#include <stdio.h>

#define P(A,B) A ## B ; bob
#define Q(A,B) A ## B+

int test(void)
{
    int bob, jim = 21;
    bob = P(jim,) *= 2;
    printf("jim: %d, bob: %d\n", jim, bob);
    jim = 60 Q(+,)3;
    printf("jim: %d\n", jim);
    return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}