#include <stdio.h>
struct st{
  int a;
};



int main() {
  struct st obj1;
  obj1.a = 6;
  printf("%d\n", obj1.a);
  return 0;
}
