#include <stdio.h>
#undef  line
#define line 1000

#line line
#if 1000 != __LINE__
	#error "  # line line" not work as expected
#endif

int
test()
{
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}