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
struct keyring_list *p_krl;

int main() {
  p_krl = & krl;

  p_krl->ptr[0] = &k0;
  p_krl->ptr[1] = &k1;
  p_krl->ptr[2] = &k2;

  for(int i = 0; i < 3; ++i) {
    printf("p_krl->ptr[%d].t = %d\n", i, p_krl->ptr[i]->t);
  }

  return 0;
}
