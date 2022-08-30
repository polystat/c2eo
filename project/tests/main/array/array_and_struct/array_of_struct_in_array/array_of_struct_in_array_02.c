//clang -Xclang -ast-dump -fsyntax-only array_of_struct_02.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_of_struct_02.c
//clang -emit-llvm -S -o array_of_struct_02.ll array_of_struct_02.c

#include <stdio.h>

struct Triangle { int side[3];} trian_array[5][3];
int i, j;
int main() {
  int count = 0;
  for(i = 0; i < 5; ++i) {
    for(j = 0; j < 3; ++j) {
      trian_array[i][j].side[0] = 3 + i + j;
      trian_array[i][j].side[1] = 4 + i + j;
      trian_array[i][j].side[2] = 5 + i + j;
      printf("trian %d: perimeter = %d\n", ++count,
           trian_array[i][j].side[0]
            +trian_array[i][j].side[1]
            +trian_array[i][j].side[2]);
    }
  }
  return 0;
}
