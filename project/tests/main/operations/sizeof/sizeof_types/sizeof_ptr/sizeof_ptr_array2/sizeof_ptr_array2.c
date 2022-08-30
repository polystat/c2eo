//clang -Xclang -ast-dump -fsyntax-only sizeof_variing.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none sizeof_variing.c
//clang -emit-llvm -S -o sizeof_variing.ll sizeof_variing.c

#include <stdio.h>

struct Rectangle {int x, y;};
struct Triangle {int a, b, c;};

unsigned long size;
int *ptr_array_int[10][3];
long *ptr_array_long[10][3];
struct Rectangle *ptr_array_struct_rect[3][10];
struct Triangle *ptr_array_struct_trian[10][3];


int main() {
  size = sizeof(ptr_array_int);
  printf("sizeof(int *ptr_array_int) = %ld\n", size);

  size = sizeof(ptr_array_long);
  printf("sizeof(int *ptr_array_long) = %ld\n", size);

  size = sizeof(ptr_array_struct_rect);
  printf("sizeof(struct Rectangle *ptr_array_struct_rect) = %ld\n", size);

  size = sizeof(ptr_array_struct_trian);
  printf("sizeof(struct Triangle *ptr_array_struct_trian) = %ld\n", size);

  return 0;
}
