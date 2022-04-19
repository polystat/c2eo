#include <stdio.h>
struct {
	enum { X } x;
} s;


int
test()
{
	return X;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}