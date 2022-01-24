#include <stdio.h>

char c = 'a';

int main() {
    printf("%c", *&c);
    return 0;
}