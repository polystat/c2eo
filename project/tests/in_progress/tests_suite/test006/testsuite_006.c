#include <stdio.h>
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
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}