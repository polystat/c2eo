#include <stdio.h>

struct key {int t;};

struct keyring_list {
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
    struct key *q;
    q = (keyring->arr)+loop;
    key_put(q);
  }
}

unsigned short x = 3;
struct keyring_list krl = {{10,20,30}};

int main() {
//   krl.arr[0].t = 10;
//   krl.arr[1].t = 20;
//   krl.arr[2].t = 30;

  for(int i = 0; i < 3; ++i) {
    printf("key_put: t = %d\n", krl.arr[i].t);
  }

  keyring_destroy(&krl, x);

  return 0;
}
