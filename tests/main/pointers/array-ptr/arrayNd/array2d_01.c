#include "stdio.h"

long a[2][3] = {0,1,2,10,11,12};
int i, j;

int main() {
//   printf("sizeof a = %ld\n", sizeof(a));
//   printf("sizeof *a = %ld\n", sizeof(*a));
//   printf("sizeof **a = %ld\n", sizeof(**a));

  printf("a[0][0] = %ld\n", a[0][0]);
  printf("a[0][1] = %ld\n", a[0][1]);
  printf("a[0][2] = %ld\n", a[0][2]);
  printf("a[1][0] = %ld\n", a[1][0]);
  printf("a[1][1] = %ld\n", a[1][1]);
  printf("a[1][2] = %ld\n", a[1][2]);

  for(i = 0; i < 2; ++i)
    for(j = 0; j < 3; ++j) {
      a[i][j] += 100;
      printf("a[%d][%d] = %ld\n", i, j, a[i][j]);
    }
  return 0;
}
