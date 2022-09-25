#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *keys[3];
};

void key_put(struct key* k) {
  printf("key_put: t = %d\n", k->t);
}

struct key *q;
struct keyring_list *keyring;

unsigned short x = 2;
struct key k;
struct keyring_list krl;
int arr[3];

int main() {
  krl.keys[0] = &k;
  k.t = 5;
  key_put(krl.keys[0]);

  keyring = &krl;
  q = keyring->keys[0];

  return 0;
}
