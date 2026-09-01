#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *keys[3];
};

// struct key *k;
struct key k;

void key_put(struct key* k) {
  printf("key_put: t = %d\n", k->t);
}

void keyring_destroy(struct keyring_list *keyring, unsigned short a)
{
 int loop;
  for (loop = a - 1; loop >= 0; loop--) {
    //key_put(keyring->keys[loop]);
    struct key *q = keyring->keys[loop];
    key_put(q);
  }
}

unsigned short x = 3;
struct keyring_list krl;

int main() {
  krl.keys[0] = &k;
  krl.keys[1] = &k;
  krl.keys[2] = &k;
  k.t = 5;
  keyring_destroy(&krl, x);

  return 0;
}
