//clang -Xclang -ast-dump -fsyntax-only array_of_struct_01.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_of_struct_01.c
//clang -emit-llvm -S -o array_of_struct_01.ll array_of_struct_01.c

#include <stdio.h>

struct Triangle { int side[3];} trian_array[5];
int i;

int main() {
  int count = 0;
  for(i = 0; i < 5; ++i) {
    trian_array[i].side[0] = 3 + i;
    trian_array[i].side[1] = 4 + i;
    trian_array[i].side[2] = 5 + i;
    printf("trian %d: perimeter = %d\n", ++count,
            trian_array[i].side[0]
          + trian_array[i].side[1]
          + trian_array[i].side[2] );
  }
  return 0; // trian_array[i].a;
}
