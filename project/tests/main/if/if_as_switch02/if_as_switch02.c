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
  int flag = 0;
  if (x == 1) {
    op1();
    return;
  }
    if ((x == 2) || (x == 3)) {
//     if ((x == 2) | (x == 3)) {
    op23();
    return;
  }
    if ((x == 4) && !(x == 5)){
//     if ((x == 4) & !(x == 5)){
    op4();
  }
    if ((x == 4) || (x == 5)) {
//     if ((x == 4) | (x == 5)) {
    op5();
    return;
  }
  if (x == 6) {
    ;
  }
  // default
  op10();
  return;
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
