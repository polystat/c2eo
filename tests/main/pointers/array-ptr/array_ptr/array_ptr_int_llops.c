//clang -Xclang -ast-dump -fsyntax-only array_ptr04.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_ptr04.c
//clang -emit-llvm -S -o array_ptr04.ll array_ptr04.c

#include <stdio.h>
int array[5] = {10, 11, 12, 13, 14};
int *ptr_int01;
int *ptr_int02;
int x = 1;

int main() {
  ptr_int01 = array;

  while(ptr_int01 < array+5) {
    printf("*ptr_int01 = %d\n", *ptr_int01);
    ptr_int01 += 1;
  }

  do {
    ptr_int01 -= 1;
    printf("*ptr_int01 = %d\n", *ptr_int01);
  } while(ptr_int01 != array);

  return 0;
}
