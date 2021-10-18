#include <stdio.h>
#include <stdlib.h>

int n;
int result = 1;

void factorial() {
    if(n < 2) {
        return;
    } else {
        result = result * n;
        n = n - 1;
        factorial();
    }
}

int main(int argc, char** argv) {
    n = atoi(argv[1]);
    printf("%d! = ", n);
    factorial();
    printf("%d\n", result);
}
