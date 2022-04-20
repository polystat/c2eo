#include <stdio.h>
#define x f
#define y() f

typedef struct { int f; } S;

int
test()
{
	S s;

	s.x = 0;
	return s.y();
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}