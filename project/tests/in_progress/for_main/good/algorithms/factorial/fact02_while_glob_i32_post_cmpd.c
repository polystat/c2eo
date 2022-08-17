#include <stdio.h>

int n = 5;

int main() {
    int f = 1;
    while(n > 1) {
        f *= n--;
    }
    printf("%d\n", f);
    f;

    return 0;
}
