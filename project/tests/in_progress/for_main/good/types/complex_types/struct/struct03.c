//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
struct st{
  int a;
};



int main() {
  struct st obj1;
  obj1.a = 6;
  obj1.a;
  printf("%d\n", obj1.a);
  return 0;
}
