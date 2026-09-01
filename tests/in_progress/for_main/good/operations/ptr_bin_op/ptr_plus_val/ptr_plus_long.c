//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long array[3] = {0, 1, 2};
long *ptr;

int main() {
  ptr = array;
  printf("array[0] = %ld\n", *ptr);
  ptr = array + 2;
  printf("array[2] = %ld\n", *ptr);
  --ptr;
  printf("array[1] = %ld\n", *ptr);

  return 0;
}
