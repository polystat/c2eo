//clang -Xclang -ast-dump -fsyntax-only main.cpp
//#include <stdio.h>

long long x = 4;
long long y = 40;
int main() {
    y /= x; // 10
    y;
//    printf("%d\n", y);
    y %= 7; // 3
    y;
//    printf("%d\n", y);
}
