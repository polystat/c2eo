//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
long long x = 10;

int main() {
  for (long long i=0; i<x; i++)
  {
    printf("%d\n",i);
  }
}
