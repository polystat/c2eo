//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
struct st{
  int a;
};

struct st obj1;

int main() {
  obj1.a = 5;
  obj1.a;
  printf("%d\n", obj1.a);
  return 0;
}
