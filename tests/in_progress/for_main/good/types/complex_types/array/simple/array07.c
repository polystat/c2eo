#include <stdio.h>
int tmp;

int main(void) {
  int arr[5] = {10, 20, 30, 40, 50};

  tmp = *arr;
  printf("%d\n", tmp);
  tmp = arr[0];
  printf("%d\n", tmp);

  return 0;
}
