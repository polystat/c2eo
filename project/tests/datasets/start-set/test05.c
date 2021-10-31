//clang -Xclang -ast-dump -fsyntax-only test05.c

int a = 2;
double y;

int main() {
    a = a + 2 * (3 - 1);
    a;
    return 0;
}