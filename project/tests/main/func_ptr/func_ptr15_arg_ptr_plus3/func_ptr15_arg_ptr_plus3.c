#include <stdio.h>
int foo(int *px) {
  return *px;
}
int (*p_foo)(int *);

int a[3] = {10, 20, 30};
int *pa;

int main() {
  pa = a;
  int b = foo(pa);
  printf("foo(pa) = %d\n", b);
  printf("foo(pa+2) = %d\n", foo(pa+2));
  printf("foo(a[1]) = %d\n", foo(&a[1]));

  p_foo = foo;
  printf("p_foo(pa) = %d\n", p_foo(pa));
  printf("p_foo(pa+2) = %d\n", p_foo(pa+2));
  printf("p_foo(a[1]) = %d\n", p_foo(&a[1]));

  return b;
}
