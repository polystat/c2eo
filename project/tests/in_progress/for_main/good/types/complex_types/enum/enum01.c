//clang -Xclang -ast-dump -fsyntax-only main.c
enum a {a0, a1, a2};
enum a v1, v2, v3;

int main() {
    v1 = a0;
    v2 = a1;
    v3 = a2;
    printf("%d\n", v1);
    printf("%d\n", v2);
    printf("%d\n", v3);
    return 0;
}
