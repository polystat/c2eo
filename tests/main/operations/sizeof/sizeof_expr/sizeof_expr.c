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

int i;
char c = 65;
long li;

int arr1[10];
array10 arr2;
array10 arr3[5];

int main() {
  struct rectangle sr;
  struct triangle  st;
  struct figure    sf;
  union mix        um;
  rectangle        rr;
  int              iloc[15];
  rectangle        rloc[3];
//   unsigned long size;
  long size;

  size = sizeof(i);
  printf("sizeof(int i) = %ld\n", size);
  size = sizeof(c);
  printf("sizeof(char c) = %ld\n", size);
  size = sizeof(li);
  printf("sizeof(long li) = %ld\n", size);

  size = sizeof(sr);
  printf("sizeof(struct rectangle sr) = %ld\n", size);
  size = sizeof(st);
  printf("sizeof(struct triangle st) = %ld\n", size);
  size = sizeof(sf);
  printf("sizeof(struct figure sf) = %ld\n", size);
  size = sizeof(um);
  printf("sizeof(union mix um) = %ld\n", size);

  size = sizeof(arr1);
  printf("sizeof(int a[10]) = %ld\n", size);
  size = sizeof(arr2);
  printf("sizeof(array10 arr2) = %ld\n", size);
  size = sizeof(arr3);
  printf("sizeof(array10 arr3[5]) = %ld\n", size);

  size = sizeof(rr);
  printf("sizeof(rectangle rr) = %ld\n", size);
  size = sizeof(iloc);
  printf("sizeof(int iloc[10]) = %ld\n", size);
  size = sizeof(rloc);
  printf("sizeof(rectangle rloc[3]) = %ld\n", size);

//   printf("size = %lu\n", size);
  return 0;
}
