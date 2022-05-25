#include <stdio.h>
int
test()
{
	int x;

	x = 50;
	do 
		x = x - 1;
	while(x);
	return x;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}