#include <stdio.h>
int
test(void)
{
	long long i;
	unsigned long long u;

	i = 1;
	i = -1;
	i = -1l;
	i = -1u;
	i = -1ll;
	i = -1ll & 3;
	i = -1ll < 0;

	u = 1;
	u = -1;
	u = -1l;
	u = -1u;
	u = -1ll;
	u = -1llu & 3;
	u = -1llu < 0;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}