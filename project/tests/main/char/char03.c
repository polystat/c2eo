//clang -Xclang -ast-dump -fsyntax-only char03.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none char03.c
//clang -emit-llvm -S -o char03.ll char03.c

#include "stdio.h"
char c = 'h';
int a = 5;
char str[] = "hell";
int main() {
  printf("a = %d;\n", a);
  printf("c = %d;\n", c);
//   printf("a = %c;\n", a);
//   printf("c = %c;\n", c);
  printf("str = %s;\n", str);
  printf("str[2] = %d;\n", str[2]);

  printf("a = %d;\nc = %d;\nstr = %s;\nstr[2] = %d;\n", a, c, str, str[2]);

  printf("a = %d;\nc = %d;\nstr = %s;\nstr[2] = %d;\n"
         "a = %d;\nc = %d;\nstr = %s;\nstr[2] = %d;\n"
         ,a, c, str, str[2]
         ,a, c, str, str[2]);

  return 0;
}
