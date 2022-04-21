#include <stdio.h>
int
test()
{
	int arr[2];
	int *p;
	
	p = &arr[1];
	*p = 0;
	return arr[1];
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}