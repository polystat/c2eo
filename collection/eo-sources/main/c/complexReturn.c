#include <stdio.h>
#include <stdlib.h>

int f(int a) {

  if (a == 1)
  {
    a = a + 1;
  }

  if (a == 2)
  {
      a = a + 2;
      return 2;
  }
  else if (a == 3)
  {
      a = a + 3;
      return 3;
  }

  if (a == 4)
  {
    a = a + 4;
  }

  a = a + 5;
  return a;
}

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = f(a);
    printf("complexReturn[%d] = %d", a, result);
}