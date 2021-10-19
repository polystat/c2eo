#include <stdio.h>

int N;

int main() {
    printf("N=?");
    scanf("%d", &N);
    int F = 1;
    while(N > 1) {
        F *= 2; //N;
        N--;
    }
    printf("Fact = %d\n", F);
    return 0;
}
