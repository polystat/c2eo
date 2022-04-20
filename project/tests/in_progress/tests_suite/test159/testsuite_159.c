#include <stdio.h>
int x = 10;

struct S {int a; int *p;};
struct S s = { .p = &x, .a = 1};

int
test()
{
	if(s.a != 1)
		return 1;
	if(*s.p != 10)
		return 2;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}