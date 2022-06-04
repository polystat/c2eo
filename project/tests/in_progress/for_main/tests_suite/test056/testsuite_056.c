#include <stdio.h>
enum E {
	x,
	y = 2,
	z,
};

int
test()
{
	enum E e;

	if(x != 0)
		return 1;
	if(y != 2)
		return 2;
	if(z != 3)
		return 3;
	
	e = x;
	return e;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}