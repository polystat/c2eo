#include <stdio.h>
struct s {
    int x;
    struct {
        int y;
        int z;
    } nest;
};

int
test() {
    struct s v;
    v.x = 1;
    v.nest.y = 2;
    v.nest.z = 3;
    if (v.x + v.nest.y + v.nest.z != 6)
        return 1;
    return 0;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}