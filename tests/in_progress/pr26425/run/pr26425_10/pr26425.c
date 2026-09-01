#include <stdio.h>

struct key {int t;};

struct keyring_list {
  struct key *ptr[3];
  struct key arr[3];
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
    key_put(keyring->ptr[loop]);
    struct key *q;
    q = keyring->ptr[loop];
    key_put(q);
    q = (keyring->arr)+loop;
    key_put(q);
  }
}

unsigned short x = 3;
struct keyring_list krl;

int main() {
  krl.ptr[0] = &k;
  krl.ptr[1] = &k;
  krl.ptr[2] = &k;
  k.t = 5;
  krl.arr[0].t = 10;
  krl.arr[1].t = 20;
  krl.arr[2].t = 30;
  keyring_destroy(&krl, x);

  return 0;
}
