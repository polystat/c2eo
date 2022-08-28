//clang -Xclang -ast-dump -fsyntax-only array_of_struct_01.c
//clang -O0 -Wall -masm=intel -S -fno-asynchronous-unwind-tables  -fcf-protection=none array_of_struct_01.c
//clang -emit-llvm -S -o array_of_struct_01.ll array_of_struct_01.c

// #include <stdio.h>

struct Triangle { int a, b, c;} trian_array[5];
int i;

int main() {
  trian_array[i].a = 33;
  int count = 0;
  for(i = 0; i < 5; ++i) {
    trian_array[i].a = 3 + i;
    trian_array[i].b = 4 + i;
    trian_array[i].c = 5 + i;
    printf("trian %d: perimeter = %d\n", ++count,
           trian_array[i].a+trian_array[i].b+trian_array[i].c);
  }
  return 0; // trian_array[i].a;
}
