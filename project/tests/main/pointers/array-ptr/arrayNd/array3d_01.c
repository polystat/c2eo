#include "stdio.h"

long a[2][3][4];
int i, j, k;

int main() {
//   printf("sizeof a = %ld\n", sizeof(a));
//   printf("sizeof *a = %ld\n", sizeof(*a));
//   printf("sizeof **a = %ld\n", sizeof(**a));
//   printf("sizeof ***a = %ld\n", sizeof(***a));


    for(i = 0; i < 2; ++i) {
      for(j = 0; j < 3; ++j) {
        for(k = 0; k < 4; ++k) {
          a[i][j][k] = 100 * i + 10 * j + k;
          printf("a[%d][%d][%d] = %ld\n", i, j, k, a[i][j][k]);
        }
      }
    }

    return 0;
}
