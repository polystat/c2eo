// Работа с указателями
// Формирование значений через void-указатели

#include <stdio.h>
int x = 5;
void *p;

int main() {
    int y;
    p = &x;
    *(int*)p = 25;
    p = &y;
    *(int*)p = x;
    printf("x = %d\ny = %d\n*p = %d\n", x, y, *(int*)p);
    return 0;
}

