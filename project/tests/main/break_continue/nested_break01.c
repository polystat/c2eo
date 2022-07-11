#include "stdio.h"
long long n = 5;
int main() {
  for (long long  i = 0; 1; ++i) {
    for (long long  j = 0; 1; ++j) {
      long long res = i * j;
      printf("%lld\n",res);
      if(j >= 5)
        break;
    }
    if(i >= 5)
      break;
  }
    return 0;
}
