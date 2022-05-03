#include <stdio.h>

struct st {
  int q;
} q2;


int main() {
  struct st1 {
    int q;
  } q;
  struct st2 {
    int q;
  };
  struct st3 {
    int q;
  } a, b, c;
  struct st4 {
    struct st5 {
      int q;
    } q;
    struct st3 q2;
  } d, e, f;
  e.q.q = 7;
  f.q2.q = 6;
  printf("%d\n", e.q.q);
  e.q.q;
  printf("%d\n", f.q2.q);
  f.q2.q;
  return 0;
}