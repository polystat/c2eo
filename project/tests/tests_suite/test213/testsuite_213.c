#include <stdio.h>
int
test()
{
	int x;
	void *foo;
	void **bar;
	
	x = 0;
	
	foo = (void*)&x;
	bar = &foo;
	
	return **(int**)bar;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}