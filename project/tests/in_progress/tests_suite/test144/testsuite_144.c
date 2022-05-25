#include <stdio.h>
extern int printf(const char *format, ...);

#define ACPI_TYPE_INVALID       0x1E
#define NUM_NS_TYPES            ACPI_TYPE_INVALID+1
int array[NUM_NS_TYPES];

#define n 0xe
int test()
{
    printf("n+1 = %d\n", n+1);
//    printf("n+1 = %d\n", 0xe+1);
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}