#include <stdio.h>

char c = 'a';

int main() {
    printf("%c", *&c);
    *&c;
    return 0;
}