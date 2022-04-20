#include <stdio.h>
typedef struct s s;

struct s {
	struct s1 {
		int s;
		struct s2 {
			int s;
		} s1;
	} s;
} s2;

#define s s

int
test(void)
{
#undef s
	goto s;
	struct s s;
		{
			int s;
			return s;
		}
	return s.s.s + s.s.s1.s;
	s:
		{
			return 0;
		}
	return 1;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}