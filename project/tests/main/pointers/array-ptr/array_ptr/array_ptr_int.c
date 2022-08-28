//clang -Xclang -ast-dump -fsyntax-only array_ptr02.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_ptr02.c
//clang -emit-llvm -S -o array_ptr02.ll array_ptr02.c

#include <stdio.h>
int array[5] = {10, 11, 12, 13, 14};
int *ptr_int01;
int *ptr_int02;
int x = 1;

int main() {
  printf("array[0] = %d\n", *array);

  ptr_int01 = &x;
  printf("*ptr_int01 = %d\n", *ptr_int01);

  ptr_int01 = array + 1;
  printf("*ptr_int01 = %d\n", *ptr_int01);

  ptr_int02 = ptr_int01 + 2;
  printf("*ptr_int02 = %d\n", *ptr_int02);

  ptr_int02 = ptr_int02 + x;
  printf("*ptr_int02 = %d\n", *ptr_int02);

  ptr_int02 = ptr_int02 - 3;
  printf("*ptr_int02 = %d\n", *ptr_int02);

  ptr_int02 = ptr_int01 - x;
  printf("*ptr_int01 = %d\n", *ptr_int01);

  *array = x;
  printf("*array = %d\n", *array);

  *(array + 1) = *(array +3);
  printf("*array = %d\n", *(array+1));

  return *ptr_int02;
}
