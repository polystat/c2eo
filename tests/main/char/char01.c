#include "stdio.h"
char c = 5;
unsigned char uc = 5;
int main() {
  char lc = 5;
  unsigned char ulc = 5;

  printf("%d\n", c);
  printf("%d\n", uc);
  printf("%d\n", lc);
  printf("%d\n", ulc);

  c++;
  uc++;
  lc++;
  ulc++;

  printf("%d\n", c);
  printf("%d\n", uc);
  printf("%d\n", lc);
  printf("%d\n", ulc);

  c--;
  uc--;
  lc--;
  ulc--;

  printf("%d\n", c);
  printf("%d\n", uc);
  printf("%d\n", lc);
  printf("%d\n", ulc);

  c += 1000;
  uc += 1000;
  lc += 1000;
  ulc += 1000;

  printf("%d\n", c);
  printf("%d\n", uc);
  printf("%d\n", lc);
  printf("%d\n", ulc);

  c -= 2000;
  uc -= 2000;
  lc -= 2000;
  ulc -= 2000;

  printf("%d\n", c);
  printf("%d\n", uc);
  printf("%d\n", lc);
  printf("%d\n", ulc);

//  printf("%c\n", c);
//  printf("%c\n", uc);
//  printf("%c\n", lc);
//  printf("%c\n", ulc);

  return 0;
}
