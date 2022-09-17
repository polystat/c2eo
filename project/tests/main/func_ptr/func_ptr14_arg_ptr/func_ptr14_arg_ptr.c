#include <stdio.h>

int foo(int *px) {
  return *px+10;
}
int (*p_foo)(int *);

int a = 10;
int *pa;

int main() {
  pa = &a;
  a = foo(pa);
  printf("foo(pa) = %d\n", a);

  p_foo = foo;
  printf("p_foo(pa) = %d\n", p_foo(pa));

  return a;
}
