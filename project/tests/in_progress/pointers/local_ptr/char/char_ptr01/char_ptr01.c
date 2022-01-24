#include <stdio.h>

int main() {
    char c = 'a';
    printf("%c", *&c);
    return 0;
}