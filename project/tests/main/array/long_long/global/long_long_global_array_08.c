#include "stdio.h"

int arr[10];
int main() {
  int i = 0;
  while (i < 10) {
    arr[i] = (i + 1) * 2;
    i += 1;
  }
  i = 0;
  while (i < 10) {
    printf("%d\n", arr[i]);
    i += 1;
  }
  return 0;
}