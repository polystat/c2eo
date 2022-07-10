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
  if (flag || x == 1) {
//   if (flag | x == 1) {
      flag = 1;
    op1();
    return;
  }
  if (flag || x == 2){
//   if (flag | x == 2){
      flag = 1;
  }
  if (flag || x == 3){
//   if (flag | x == 3){
    flag = 1;
    op23();
    return;
  }
  if (flag || x == 4){
//   if (flag | x == 4){
      flag = 1;
    op4();
  }
  if (flag || x == 5){
//   if (flag | x == 5){
    flag = 1;
    op5();
    return;
  }
  if (flag || x == 6){
//   if (flag | x == 6){
        flag = 1;
  }
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
