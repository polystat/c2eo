//Одна из функций запускается как параметр другой

#include <stdio.h>

double c = 0.0;

double func1() {
    return c - 111.0;
}

int main() {
    c = 10.0;
    double a;
    a = func1();
    printf("%f\n", a);
    return 0;
}
