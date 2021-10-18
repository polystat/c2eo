// Работа с указателями
// Формирование значений через указатели

#include <stdio.h>
int x = 5;
int *p;

int main() {
    int y;
    p = &x;
    *p = 25;
    p = &y;
    *p = x;
    printf("x = %d\ny = %d\n*p = %d\n", x, y, *p);
    return 0;
}
