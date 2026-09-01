#include <stdio.h>

int main() {
  int a = 1024;
  int b = -1024;
  int c = 0;
  for (int i = 0; i < 5; i++) {
    printf("%d\n", a >> i);
    printf("%d\n", b >> i);
    printf("%d\n", c >> i);
    printf("%d\n", a << i);
    printf("%d\n", b << i);
    printf("%d\n", c << i);
  } 
}
