#include <stdio.h>
struct S1 {
	int a;
	int b;
};

struct S2 {
	int a;
	int b;
	union {
		int c;
		int d;
	};
	struct S1 s;
};

struct S2 v/* = {1, 2, 3, {4, 5}}*/;

int main () {
  v.a = 1;
  v.b = 2;
  v.c = 3;
  v.s.a = 4;
  v.s.b = 5;

  printf("%d\n", v.a);
  v.a;
  printf("%d\n", v.b);
  v.b;
  printf("%d\n", v.c);
  v.c;
  printf("%d\n", v.d);
  v.d;
  printf("%d\n", v.s.a);
  v.s.a;
  printf("%d\n", v.s.b);
  v.s.b;

  return 0;
}