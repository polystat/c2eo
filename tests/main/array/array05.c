#include <stdio.h>

int main(void) {
  int tmp;
  int arr[10] = {10, 20};
  for (int i = 0; i < 10; ++i) {
    tmp = arr[i];
    printf("%d\n", tmp);
  } // 10 20 ?
  return 0;
}
