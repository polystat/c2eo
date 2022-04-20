#include <stdio.h>
int
test()
{
	int i;

	for(i = 0; i < 10; i++)
		if (!i)
			continue;
	
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}