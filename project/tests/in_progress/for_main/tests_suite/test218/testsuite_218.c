#include <stdio.h>
#define M(x) x
#define A(a,b) a(b)

int
test(void)
{
	char *a = A(M,"hi");

	return (a[1] == 'i') ? 0 : 1;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}