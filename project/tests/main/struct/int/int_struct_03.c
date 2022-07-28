#include <stdio.h>
struct st{
  int a;
};

struct st obj1;

int main() {
  obj1.a = 5;
  printf("%d\n", obj1.a);
  return 0;
}
