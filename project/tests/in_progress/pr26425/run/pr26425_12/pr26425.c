#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key arr[3];
};

struct key k;

unsigned short x = 3;
struct keyring_list krl = {10,20,30};

int main() {
//   krl.arr[0].t = 10;
//   krl.arr[1].t = 20;
//   krl.arr[2].t = 30;

  for(int i = 0; i < 3; ++i) {
    printf("krl.arr[%d].t = %d\n", i, krl.arr[i].t);
  }

  return 0;
}
