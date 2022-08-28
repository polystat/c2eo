#include <stdio.h>

int x;
int t;

void op1() {
  t = 1;
  printf("%d\n", t);
}
void op23() {
  t = 23;
  printf("%d\n", t);
}
void op4() {
  t = 4;
  printf("%d\n", t);
}
void op5() {
  t = 5;
  printf("%d\n", t);
}
void op10() {
  t = 10;
  printf("%d\n", t);
}

void test_switch() {
  switch (x) {
  case 1:
    op1();
    break;
  case 2:
  case 3:
    op23();
    break;
  case 4:
    op4();
  case 5:
    op5();
    break;
  case 6:
  default:
    op10();
    break;
  }
}

int main() {
  x = 0;
  printf("%d\n", x);
  test_switch();
  x = 1;
  printf("%d\n", x);
  test_switch();
  x = 2;
  printf("%d\n", x);
  test_switch();
  x = 3;
  printf("%d\n", x);
  test_switch();
  x = 4;
  printf("%d\n", x);
  test_switch();
  x = 5;
  printf("%d\n", x);
  test_switch();
  x = 6;
  printf("%d\n", x);
  test_switch();
  x = 7;
  printf("%d\n", x);
  test_switch();
}
