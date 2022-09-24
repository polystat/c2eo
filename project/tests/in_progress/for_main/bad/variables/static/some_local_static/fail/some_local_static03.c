#include <stdio.h>

void foo1() {
  static int a = 10;
  a++;
  printf("a-foo1-1 = %d\n", a);
  {
    static int a = 30;
    a++;
    printf("a-foo1-2 = %d\n", a);
  }
}

void foo2() {
  static int a = 20;
  a++;
  printf("a-foo2-1 = %d\n", a);
  {
    static int a = 40;
    a++;
    printf("a-foo2-2 = %d\n", a);
  }
}

static int a = 50;

int main(){
  a++;
  foo1();
  foo2();
  printf("a-glob-static = %d\n", a);
  return 0;
}
