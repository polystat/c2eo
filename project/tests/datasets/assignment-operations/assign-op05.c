//clang -Xclang -ast-dump -fsyntax-only main.cpp
//#include <stdio.h>

int main() {
    long long x = 22; // 10110
    long long y = 7, y1 = 7, y2 = 7; // 00111

    y <<= 1;
    y += 5;
    y -= 11; // 0
    y;
//    printf("%d\n", y);
}
