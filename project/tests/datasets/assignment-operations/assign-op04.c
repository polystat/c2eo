//clang -Xclang -ast-dump -fsyntax-only main.cpp
//#include <stdio.h>

long long x = 22; // 10110
long long y = 7, y1 = 7, y2 = 7; // 00111
int main() {
    y <<= 1; // 6
    y;
//    printf("%d\n", y);
    y2 <<= 3; // 2
    y1;
//    printf("%d\n", y1);
    y2 >>= 2; // 5
    y2;
//    printf("%d\n", y2);
}
