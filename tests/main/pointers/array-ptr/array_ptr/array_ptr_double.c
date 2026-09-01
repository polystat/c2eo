//clang -Xclang -ast-dump -fsyntax-only array_ptr03.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_ptr03.c
//clang -emit-llvm -S -o array_ptr03.ll array_ptr03.c

#include <stdio.h>
double array[5] = {10.0, 11.0, 12.0, 13.0, 14.0};
double *ptr_double01;
double *ptr_double02;
int x = 1;

int main() {
  printf("array[0] = %f\n", *array);

  ptr_double01 = &x;
  printf("*ptr_double01 = %f\n", *ptr_double01);

  ptr_double01 = array + 1;
  printf("*ptr_double01 = %f\n", *ptr_double01);

  ptr_double02 = ptr_double01 + 2;
  printf("*ptr_double02 = %f\n", *ptr_double02);

  ptr_double02 = ptr_double02 + x;
  printf("*ptr_double02 = %f\n", *ptr_double02);

  ptr_double02 = ptr_double02 - 3;
  printf("*ptr_double02 = %f\n", *ptr_double02);

  ptr_double02 = ptr_double01 - x;
  printf("*ptr_double01 = %f\n", *ptr_double01);

  return 0;
}
