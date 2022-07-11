#include <stdio.h>

// int arr[] = {10, 20};

int main(void) {
  int tmp;
  int arr[] = {10, 20};
  for (int i = 0; i < 2; ++i) {
    tmp = arr[i];
    printf("%d\n", tmp);
  } // 10 20 ?

  return 0;
}
