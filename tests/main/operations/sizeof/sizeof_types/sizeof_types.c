//clang -Xclang -ast-dump -fsyntax-only sizeof_int.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none sizeof_int.c
//clang -emit-llvm -S -o sizeof_int.ll sizeof_int.c

#include <stdio.h>

typedef struct rectangle {int x, y;} rectangle;
struct triangle {int a, b, c;};
struct figure {
  int key;
  union {
    struct rectangle r;
    struct triangle t;
  };
};

union mix {
  struct rectangle r;
  struct triangle t;
};

typedef int array10[10];

int main() {
//   unsigned long size;

  long size;
  size = sizeof(int);
  printf("sizeof(int) = %ld\n", size);
  size = sizeof(char);
  printf("sizeof(char) = %ld\n", size);
  size = sizeof(long int);
  printf("sizeof(long int) = %ld\n", size);

  size = sizeof(struct rectangle);
  printf("sizeof(struct rectangle) = %ld\n", size);
  size = sizeof(struct triangle);
  printf("sizeof(struct triangle) = %ld\n", size);
  size = sizeof(struct figure);
  printf("sizeof(struct figure) = %ld\n", size);
  size = sizeof(union mix);
  printf("sizeof(union mix) = %ld\n", size);

  size = sizeof(array10);
  printf("sizeof(array10) = %ld\n", size);
  size = sizeof(rectangle);
  printf("sizeof(rectangle) = %ld\n", size);

  //   printf("size = %lu\n", size);
  return 0;
}
