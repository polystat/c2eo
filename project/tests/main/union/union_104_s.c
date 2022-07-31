#include <stdio.h>
struct st{
	int a;
	union {
		int b1;
		int b2;
	} b;
	struct { union { struct { int c; } e; } d; } c;
	struct {
		int d;
	} f;
};
typedef struct st st;

int main () {
  st v;

  v.a = 1;
  v.b.b1 = 2;
  v.c.d.e.c = 3;
  v.f.d = 4;

  printf("%d\n", v.a);
  printf("%d\n", v.b.b1);
  printf("%d\n", v.b.b2);
  printf("%d\n", v.c.d.e.c);
  printf("%d\n", v.f.d);

  return 0;
}
