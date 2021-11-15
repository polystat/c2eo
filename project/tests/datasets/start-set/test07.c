//clang -Xclang -ast-dump -fsyntax-only test07.c

//#include <stdio.h>

int a = 2;
int b = 3;
int c;
int main() {
    if (a > b)
    {
        c = a;
    }
    c;
}
