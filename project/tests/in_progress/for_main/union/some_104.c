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


int main () {
  s v;

  v.a = 1;
  v.b1 = 2;
  v.c = 3;
  v.d = 4;

  printf("%d\n", v.a);
  printf("%d\n", v.b1);
  printf("%d\n", v.b2);
  printf("%d\n", v.c);
  printf("%d\n", v.d);

  return 0;
}
