#include <stdio.h>

int
test(void)
{
	printf("hello world\n");
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}