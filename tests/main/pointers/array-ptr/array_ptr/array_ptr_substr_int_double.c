//clang -Xclang -ast-dump -fsyntax-only array_ptr04.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_ptr04.c
//clang -emit-llvm -S -o array_ptr04.ll array_ptr04.c

#include <stdio.h>
int array[5] = {10, 11, 12, 13, 14};
int *ptr_int01;
int *ptr_int02;
double *ptr_double01;
double darray[5] = {110, 111, 112, 113, 114};
int x = 1;

int main() {
  ptr_int01 = array + 1;
  printf("*ptr_int01 = %d\n", *ptr_int01);

  ptr_int02 = ptr_int01 + 3;
  printf("*ptr_int02 = %d\n", *ptr_int02);

  x = ptr_int02 - ptr_int01;
  printf("x = %d\n", x);

  x = ptr_int01 - ptr_int02;
  printf("x = %d\n", x);

  x = ptr_int02 - array;
  printf("x = %d\n", x);

  x = array - ptr_int02;
  printf("x = %d\n", x);

  ptr_double01 = darray + 2;
  printf("*ptr_double01 = %f\n", *ptr_double01);

  x = ptr_double01 - darray;
  printf("x = %d\n", x);

  return x;
}
