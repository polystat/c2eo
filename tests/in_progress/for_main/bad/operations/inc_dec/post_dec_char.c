//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

char e = 'z';

int main() {
  e--;
  printf("%c\n", e);
  e;

  return 0;
}
