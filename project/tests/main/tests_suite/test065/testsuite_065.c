#include <stdio.h>
#define BAR 0
#ifdef BAR
	#ifdef FOO
		XXX
		#ifdef FOO
			XXX
		#endif
	#else
		#define FOO
		#ifdef FOO
			int x = BAR;
		#endif
	#endif
#endif

int
test()
{
	return BAR;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}