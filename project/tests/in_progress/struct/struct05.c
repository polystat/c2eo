//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
typedef struct st st;
struct st{
  int a:1, b:2, c:3,:5,d:6;
};

st obj1;

int main() {
  obj1.a = -2;
  obj1.b = -3;
  obj1.c = -5;
  obj1.d = -33;
  printf("%d\n", obj1.a);
  obj1.a;
  printf("%d\n", obj1.b);
  obj1.b;
  printf("%d\n", obj1.c);
  obj1.c;
  printf("%d\n", obj1.d);
  obj1.d;
  return 0;
}
