#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *ptr[3];
};

struct key k;

unsigned short x = 3;
struct keyring_list krl;

struct key k0 = {10};
struct key k1 = {20};
struct key k2 = {30};

struct keyring_list krl;

int main() {

  krl.ptr[0] = &k0;
  krl.ptr[1] = &k1;
  krl.ptr[2] = &k2;

  for(int i = 0; i < 3; ++i) {
    printf("krl.ptr[%d].t = %d\n", i, krl.ptr[i]->t);
  }

  return 0;
}
