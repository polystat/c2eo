#include <stdio.h>

char c = 'k';
char c1 =  55;
char *c2 =  &c;

int main() {
    printf("%c\n", *&c);
    *&c;
    printf("%c\n", *&c1);
    *&c1;
    printf("%c\n", *c2);
    *c2;
    return 0;
}