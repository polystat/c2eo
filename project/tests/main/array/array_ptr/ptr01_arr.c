#include <stdio.h>

long long A[10];
long long *ptr;
// long long x;

int main() {
  long long i;
  long long n;
  i = 0;
  n = 10;

  while(i < n) {
    A[i] = i;
    printf("%lld\n", A[i]);
    ++i;
  }

   long long x;
  i = 0;
  ptr = &A;
  while(i < n) {
     x = *ptr;
     printf("%lld\n", x);
    ++ptr;
    ++i;
 }

  return 0;
}
