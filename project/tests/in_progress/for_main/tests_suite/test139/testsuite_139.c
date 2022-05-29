#include <stdio.h>
int arr[3] = {[2] = 2, [0] = 0, [1] = 1};

int
test()
{
	if(arr[0] != 0)
		return 1;
	if(arr[1] != 1)
		return 2;
	if(arr[2] != 2)
		return 3;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}