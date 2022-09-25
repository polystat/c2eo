#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *keys[3];
};

// struct key k;
struct key *q;

struct keyring_list krl;
struct keyring_list *keyring;

struct key k;
struct key *q;

unsigned short x = 2;
struct keyring_list krl;

int main() {
  krl.keys[0] = &k;
  k.t = 5;
  printf("key_put: t = %d\n", k.t);
  printf("krl.keys[0]->t = %d\n", krl.keys[0]->t);

  keyring = &krl;

  q = krl.keys[0];
  printf("q.keys[0]->t = %d\n", q->t);

//   q = *(krl.keys);
//   printf("*(krl.keys)->t = %d\n", q->t);

//   q = *(keyring->keys);
//   printf("q->keys[0]->t = %d\n", q->t);

//   q = (keyring->keys)[0];
//   printf("(keyring->keys)[0]->t = %d\n", q->t);

//   q = keyring->keys[0];
//   printf("q->keys[0]->t = %d\n", q->t);

  return 0;
}
