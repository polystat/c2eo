#include <stdio.h>

// void fu(int arr1[3][3], int arr[3]){
//   printf("ok");
// }

int main() {

  int n = 3;
  int a[3][3][3] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
  printf("[%d,%d,%d]: %d\n", 0, 0, 0, a[0][0][0]);
  printf("[%d,%d,%d]: %d\n", 0, 0, 1, a[0][0][1]);
  printf("[%d,%d,%d]: %d\n", 0, 0, 2, a[0][0][2]);
  printf("[%d,%d,%d]: %d\n", 0, 1, 0, a[0][1][0]);
  printf("[%d,%d,%d]: %d\n", 0, 1, 1, a[0][1][1]);
  printf("[%d,%d,%d]: %d\n", 0, 1, 2, a[0][1][2]);
//  printf("[%d,%d,%d]: %d\n", 0, 2, 0, a[0][2][0]);
//  printf("[%d,%d,%d]: %d\n", 0, 2, 1, a[0][2][1]);
//  printf("[%d,%d,%d]: %d\n", 0, 2, 2, a[0][2][2]);
//  printf("[%d,%d,%d]: %d\n", 1, 0, 0, a[1][0][0]);
//  printf("[%d,%d,%d]: %d\n", 1, 0, 1, a[1][0][1]);
//  printf("[%d,%d,%d]: %d\n", 1, 0, 2, a[1][0][2]);
//  printf("[%d,%d,%d]: %d\n", 1, 1, 0, a[1][1][0]);
//  printf("[%d,%d,%d]: %d\n", 1, 1, 1, a[1][1][1]);
//  printf("[%d,%d,%d]: %d\n", 1, 1, 2, a[1][1][2]);
//  printf("[%d,%d,%d]: %d\n", 1, 2, 0, a[1][2][0]);
//  printf("[%d,%d,%d]: %d\n", 1, 2, 1, a[1][2][1]);
//  printf("[%d,%d,%d]: %d\n", 1, 2, 2, a[1][2][2]);
//  printf("[%d,%d,%d]: %d\n", 2, 0, 0, a[2][0][0]);
//  printf("[%d,%d,%d]: %d\n", 2, 0, 1, a[2][0][1]);
//  printf("[%d,%d,%d]: %d\n", 2, 0, 2, a[2][0][2]);
//  printf("[%d,%d,%d]: %d\n", 2, 1, 0, a[2][1][0]);
//  printf("[%d,%d,%d]: %d\n", 2, 1, 1, a[2][1][1]);
//  printf("[%d,%d,%d]: %d\n", 2, 1, 2, a[2][1][2]);
//  printf("[%d,%d,%d]: %d\n", 2, 2, 0, a[2][2][0]);
//  printf("[%d,%d,%d]: %d\n", 2, 2, 1, a[2][2][1]);
//  printf("[%d,%d,%d]: %d\n", 2, 2, 2, a[2][2][2]);

  return 0;
}