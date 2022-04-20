#include <stdio.h>
typedef struct {
	int v;
	int sub[2];
} S;

S a[1] = {{1, {2, 3}}};

int
test()
{
	if (a[0].v != 1)
		return 1;
	if (a[0].sub[0] != 2)
		return 2;
	if (a[0].sub[1] != 3)
		return 3;
	
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}