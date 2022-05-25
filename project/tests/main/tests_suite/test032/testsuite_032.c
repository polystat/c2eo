#include <stdio.h>
// line comment

int
test()
{
	/*
		multiline
		comment
	*/
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}