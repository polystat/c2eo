#include <stdio.h>
#include <stdlib.h>

int n;
int i = 0;
float divider = 1.0;
float result = 0;

void pi() {
    if(i < n) {
        result = result + (1.0 / ((i % 2 == 0) ? divider : -divider));
        divider = divider + 2.0;
        i = i + 1;
        pi();
    } else {
        result = result * 4.0;
        return;
    }
}

int main(int argc, char** argv) {
    n = atoi(argv[1]);
    printf("pi[%d] = ", n);
    pi();
    printf("%f\n", result);
}