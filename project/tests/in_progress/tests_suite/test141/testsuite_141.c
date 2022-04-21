#include <stdio.h>
void
voidfn()
{
    return;
}

int
test()
{
    voidfn();
    return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}