#include <stdio.h>
#include <stdlib.h>

int n;
int result = 1;
int lastResult = 0;
int tmp;

void fibonacci() {
    if(n < 1) {
        return;
    } else {
        tmp = result;
        result = result + lastResult;
        lastResult = tmp;
        n = n - 1;
        fibonacci();
    }
}

int main(int argc, char** argv) {
    n = atoi(argv[1]);
    printf("fibonacci[%d] = ", n);
    fibonacci();
    printf("%d\n", lastResult);
}