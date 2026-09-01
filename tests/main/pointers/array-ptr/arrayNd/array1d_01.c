//clang -Xclang -ast-dump -fsyntax-only array_ptr01.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_ptr01.c
//clang -emit-llvm -S -o array_ptr01.ll array_ptr01.c

#include <stdio.h>
int array[5];
int x;

int main() {
  array[0] = 10;
  array[1] = 11;
  array[2] = 12;
  array[3] = 13;
  array[4] = 14;
  for(int i = 0; i < 5; ++i) {
    printf("array[%d] = %d\n", i, array[i]);
  }

  return array[0];
}
