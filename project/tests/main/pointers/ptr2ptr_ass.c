//clang -Xclang -ast-dump -fsyntax-only main.c
//#include <stdio.h>

// Проверка инициализации указателей
long long c = 255;
// long long d = 10;
long long *pc; // = &c;
long long *pd; // = &d;
// long long **ppc; // = &c;

int main() {
  pd = pc;
//   printf("%lld\n", c);
//   c;

  return 0;
}
