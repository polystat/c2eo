#include <stdio.h>
int
test()
{
	return L'\0';
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}