#include <stdio.h>

int i = 0;

long long func1() {
    printf("func1\n");
}

long long func2() {
    printf("func2\n");
}

int main() {
    func1();
    func2();
    if (i < 10) {
      i = i + 1;
      main();
    }
    return 0;
}
