# Реализация алгоритма обмена данными между двумя переменными через третью

B <=> C == A = B; B = C; C = A

Реализация на С:

```
#include <stdio.h>

int a = 10;
int b = 20;
int c = 30;

int main() {
    a = b;
    b = c;
    c = a;
    printf("a = %d, b = %d, c = %d\n", a, b, c);
    return 0;
}
```
