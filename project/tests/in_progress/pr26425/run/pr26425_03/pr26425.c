#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *keys[3];
};

void key_put(struct key* k) {
  printf("key_put: t = %d\n", k->t);
}

unsigned short x = 2;
struct key k;
struct keyring_list krl;
int arr[3];

int main() {
  krl.keys[0] = &k;
  k.t = 5;
  key_put(krl.keys[0]);
  printf("key_put: t = %d\n", krl.keys[0]->t);

  return 0;
}
