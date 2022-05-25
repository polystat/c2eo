#include <stdio.h>
#if defined X
X
#endif

#if defined(X)
X
#endif

#if X
X
#endif

#define X 0

#if X
X
#endif

#if defined(X)
int x = 0;
#endif

#undef X
#define X 1

#if X
int
test()
{
	return 0;
}
#endif
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}