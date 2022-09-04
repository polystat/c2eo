#include <stdio.h>

int main() {

  int n = 3;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      for (int k = 0; k < n; k++) {
        printf("[%d,%d,%d]: %d\n", i, j, k, i + j + k);
      }
  return 0;
}