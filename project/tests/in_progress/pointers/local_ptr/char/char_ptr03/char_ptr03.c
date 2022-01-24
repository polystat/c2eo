#include <stdio.h>

int main() {
    char c = 'k';
    printf("%c\n", *&c);
    *&c;
    char c1 =  55;
    printf("%c\n", *&c1);
    *&c1;
    char *c2 =  &c;
    printf("%c\n", *c2);
    *c2;
    return 0;
}