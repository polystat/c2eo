//clang -Xclang -ast-dump -fsyntax-only array_of_struct_03.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_of_struct_03.c
//clang -emit-llvm -S -o array_of_struct_03.ll array_of_struct_03.c

#include <stdio.h>

struct Triangle { int a, b, c;} trian_array[5][3][2];
int i, j, k;
int main() {
  int count = 0;
  for(i = 0; i < 5; ++i) {
    for(j = 0; j < 3; ++j) {
      for(k = 0; k < 2; ++k) {
        trian_array[i][j][k].a = 3 + i + j + k;
        trian_array[i][j][k].b = 4 + i + j + k;
        trian_array[i][j][k].c = 5 + i + j + k;
        printf("trian %d: perimeter = %d\n", ++count,
           trian_array[i][j][k].a+trian_array[i][j][k].b+trian_array[i][j][k].c);
      }
    }
  }
  return 0;
}
