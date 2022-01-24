#include <stdio.h>

int main() {
    char c = 'g';
    printf("%c\n", c);
    c;
    char *c1 = &c;
    printf("%c\n", *c1);
    *c1;
    return 0;
}