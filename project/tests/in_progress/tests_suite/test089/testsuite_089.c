#include <stdio.h>
int a[] = {1, 2, 3, 4};

int
test()
{
	if (sizeof(a) != 4*sizeof(int))
		return 1;
	
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}