// Работа с указателями
// Формирование значений через void-указатели

#include <stdio.h>
int x = 5;
int *p;
int **pp;

int main() {
    int y;
    p = &x;
    *p = 25;
    pp = &p;
    p = &y;
    y = x + 10;
    int ***ppp = &pp;
    printf("x = %d\ny = %d\n*p = %d\n**pp = %d\n***ppp = %d\n",
           x, y, *p, **pp, ***ppp);
    return 0;
}
