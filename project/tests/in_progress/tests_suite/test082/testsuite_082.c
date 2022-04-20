#include <stdio.h>
#define B "b"

char s[] = "a" B "c";

int
test()
{
	if (s[0] != 'a')
		return 1;
	if (s[1] != 'b')
		return 2;
	if (s[2] != 'c')
		return 3;
	if (s[3] != '\0')
		return 4;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}