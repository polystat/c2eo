//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
struct st{
  int a;
};

struct st obj1;

int main() {
  struct st obj2;
  obj1.a = 7;
  obj2 = obj1;
  printf("%d\n", obj2.a);
  obj2.a;
  return 0;
}
