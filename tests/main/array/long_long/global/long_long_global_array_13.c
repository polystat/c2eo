#include "stdio.h"
typedef struct st1 st1;
typedef struct st2 st2;

struct st2{
  int a, b, c[3];
};

struct st1{
  st2 a, b[3], c;
} obj[5];

int main() {
  obj[2].b[2].c[2] = 7;
  printf("%d\n", obj[2].b[2].c[2]);
  printf("%d\n", 2[obj[2].b[2].c]);
  printf("%d\n", 2[2[obj[2].b].c]);
  printf("%d\n", 2[2[2[obj].b].c]);
  return 0;
}
