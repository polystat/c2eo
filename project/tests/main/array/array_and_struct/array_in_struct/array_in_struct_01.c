//clang -Xclang -ast-dump -fsyntax-only array_in_struct_01.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_in_struct_01.c
//clang -emit-llvm -S -o array_in_struct_01.ll array_in_struct_01.c

// #include <stdio.h>

struct Triangle { int side[3];} trian;

int main() {
  trian.side[0] = 3;
  trian.side[1] = 4;
  trian.side[2] = 5;
  printf("trian: perimeter = %d\n",
          trian.side[0]+trian.side[1]+trian.side[2]);

  return 0;
}
