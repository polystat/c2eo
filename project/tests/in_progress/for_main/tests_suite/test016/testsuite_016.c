#include <stdio.h>
struct S
{
	int	(*fptr)();
};

int
foo()
{
	return 0;
}

int
test()
{
	struct S v;
	
	v.fptr = foo;
	return v.fptr();
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}