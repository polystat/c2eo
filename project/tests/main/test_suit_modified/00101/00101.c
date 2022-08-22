int
test()
{
  int c;
  c = 0;
  do
    ;
  while (0);
  return c;
}

#include <stdio.h>
int main()
{
  int x = test();
  printf("%d\n", x);
  return x;
}
