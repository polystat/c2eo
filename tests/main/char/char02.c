#include "stdio.h"
char c[] = {3, 4, 5, 6, 7, 8, 9};
char str[] = "hell";
char str2[] = {'h', 1, 2, 3, 'k'};
unsigned char str3[] = {'h', -1, -2, 3, 'k'};
int main() {
  for (int i = 0; i < 8; i++)
    printf("%d\n", c[i]);
  for (int i = 0; i < 5; i++)
    printf("%d\n", str[i]);
  for (int j = 0; j < 5; j++)
    printf("%d\n", str2[j]);
  for (int j = 0; j < 5; j++)
    printf("%d\n", str3[j]);
  return 0;
}
