#include <stdio.h>

int func01();

int (*func_ptr)();

void print_result(int r, long int a1, long int a2, long int a3) {
  printf("result = %d\n", r);
}

int main() {
  int arg1 = 12;
  double arg2 = 3.14;
  long int arg3 = 20L;
  func_ptr = func01;
  int result;

//   result = func01(12, 3.14, 20L);
//   print_result(result, 0, 0, 0);
//
//   result = func01(12, 3.14);
//   print_result(result, 0, 0, 0);
//
//   result = func01(12);
//   print_result(result, 0, 0, 0);
//
//   result = func01();
//   print_result(result, 0, 0, 0);
//
//   result = func01(arg1, arg2, arg3);
//   print_result(result, 0, 0, 0);
//
//   result = func01(arg1, arg2);
//   print_result(result, 0, 0, 0);
//
//   result = func01(arg1);
//   print_result(result, 0, 0, 0);

  result = func_ptr(12, 3.14, 20L);
  print_result(result, 0, 0, 0);

  result = func_ptr(12, 3.14);
  print_result(result, 0, 0, 0);

  result = func_ptr(12);
  print_result(result, 0, 0, 0);

  result = func_ptr();
  print_result(result, 0, 0, 0);

  result = func_ptr(arg1, arg2, arg3);
  print_result(result, 0, 0, 0);

  result = func_ptr(arg1, arg2);
  print_result(result, 0, 0, 0);

  result = func_ptr(arg1);
  print_result(result, 0, 0, 0);

  return 0;
}

int func01(int x, double y, long z) {
  if (z != 20L) {z = 3;}
  if (y != 3.14) {y = 2.7;}
  if (x != 12) {x = 1;}
  return x + y + z;
}
