#include <stdio.h>
int
test()
{
	int arr[2];

	arr[0] = 1;
	arr[1] = 2;

	return arr[0] + arr[1] - 3;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}