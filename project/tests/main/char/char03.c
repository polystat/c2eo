#include "stdio.h"
char c = 'h';
int a = 5;
char str[] = "hell";
int main() {
//  printf("a = %d;\nc = %d;\na = %c;\nc = %lc;\nstr = %s;\nstr[2] = %c;\n", a, c, a, c, str, str[2]);
  printf("a = %d;\nc = %d;\na = %d;\nc = %ld;\nstr = %s;\nstr[2] = %d;\n", a, c, a, c, str, str[2]);
  return 0;
}
