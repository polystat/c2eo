#include <stdio.h>
#define A 3
#define FOO(X,Y,Z) X + Y + Z
#define SEMI ;

int
test()
{
	if(FOO(1, 2, A) != 6)
		return 1 SEMI
	return FOO(0,0,0);
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}