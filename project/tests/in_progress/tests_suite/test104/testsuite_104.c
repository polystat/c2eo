#include <stdio.h>
typedef struct {
	int a;
	union {
		int b1;
		int b2;
	};
	struct { union { struct { int c; }; }; };
	struct {
		int d;
	};
} s;

int
test()
{
	s v;
	
	v.a = 1;
	v.b1 = 2;
	v.c = 3;
	v.d = 4;
	
	if (v.a != 1)
		return 1;
	if (v.b1 != 2 && v.b2 != 2)
		return 2;
	if (v.c != 3)
		return 3;
	if (v.d != 4)
		return 4;
	
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}