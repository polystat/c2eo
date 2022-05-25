//clang -Xclang -ast-dump -fsyntax-only main.cpp
long long x = 10;

int main() {
  long long i = 0;
  printf("%d\n",i);
  for (i=0; i<x; i++)
  {
    printf("%d\n",i);
  }
  printf("%d\n",i);
}
