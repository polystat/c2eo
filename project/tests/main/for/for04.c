//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
long long x = 10;

int main() {
  long long i;
  for (i = 0; i < x;)
  {
    printf("%lld\n", i);
    i++;
  }
  printf("%lld\n",i);
}
