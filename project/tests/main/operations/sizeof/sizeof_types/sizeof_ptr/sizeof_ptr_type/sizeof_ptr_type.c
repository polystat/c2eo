//clang -Xclang -ast-dump -fsyntax-only sizeof_ptr_int.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none sizeof_ptr_int.c
//clang -emit-llvm -S -o sizeof_ptr_int.ll sizeof_ptr_int.c

#include <stdio.h>

struct Rectangle {int x, y;};
struct Triangle {int a, b, c;};

//   unsigned long size;
unsigned long size;
int *ptr_int;
int *ptr_long;
struct Rectangle *ptr_struct_rect;
struct Triangle *ptr_struct_trian;

int main() {
  size = sizeof(ptr_int);
  printf("sizeof(int *ptr_int) = %ld\n", size);

  size = sizeof(ptr_long);
  printf("sizeof(int *ptr_long) = %ld\n", size);

  size = sizeof(ptr_struct_rect);
  printf("sizeof(struct Rectangle *ptr_struct_rect) = %ld\n", size);

  size = sizeof(ptr_struct_trian);
  printf("sizeof(struct Triangle *ptr_struct_trian) = %ld\n", size);

  return 0;
}
