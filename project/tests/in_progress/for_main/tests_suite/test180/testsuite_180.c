#include <stdio.h>
#include <stdint.h>

int
test()
{
	int32_t x;
	int64_t l;
	
	x = 0;
	l = 0;
	
	x = ~x;
	if (x != 0xffffffff)
		return 1;
	
	l = ~l;
	if (x != 0xffffffffffffffff)
		return 2;

	
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}