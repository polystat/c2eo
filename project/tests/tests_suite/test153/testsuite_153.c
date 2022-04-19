#include <stdio.h>
int arr[][3][5] = {
	{
		{ 0, 0, 3, 5 },
		{ 1, [3] = 6, 7 },
	},
	{
		{ 1, 2 },
		{ [4] = 7, },
	},
};

int
test(void)
{
	return !(arr[0][1][4] == arr[1][1][4]);
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}