#include <stdio.h>

void foo1() {
  static int a = 10;
  a++;
  printf("a-foo1 = %d\n", a);
}

void foo2() {
  static int a = 20;
  a++;
  printf("a-foo2 = %d\n", a);
}

int main(){
  foo1();
  foo2();
  return 0;
}
