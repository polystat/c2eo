#include <stdio.h>

int func04();
int func05();

int func01() {
  return 1;
}

int func02() {
  return 2;
}

int func03() {
  return 3;
}

int func04();
int func05();
int func04();
int func05();

int (*func_ptr)();

int main() {
  int result;
  func_ptr = func01;
  result = func_ptr();
  printf("func01=%d\n", result);
  func_ptr = func02;
  result = func_ptr();
  printf("func02=%d\n", result);
  func_ptr = func03;
  result = func_ptr();
  printf("func03=%d\n", result);
  func_ptr = func04;
  result = func_ptr();
  printf("func04=%d\n", result);
  func_ptr = func05;
  result = func_ptr();
  printf("func05=%d\n", result);

  return 0;
}

int func04() {
  return 4;
}

int func05() {
  return 5;
}

int func04();
int func05();
