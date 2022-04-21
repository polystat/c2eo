#include <stdio.h>
int strlen(char *);

int
test()
{
	char *p;
	
	p = "hello";
	return strlen(p) - 5;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}