#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *keys[3];
};

struct key k;
struct key *q;

struct keyring_list krl;
struct keyring_list *keyring;

int main() {
  keyring = &krl;
  keyring->keys[2] = &k;
  k.t = 5;
  q = keyring->keys[2];
  printf("q = %d\n", q->t);

  return 0;
}
