//clang -Xclang -ast-dump -fsyntax-only main.c
enum a {a0=5, a1=10, a2=-5, a3, a4=20, a5};
enum a v1, v2, v3, v4, v5, v6;

int main() {
    v1 = a0;
    v2 = a1;
    v3 = a2;
    v4 = a3;
    v5 = a4;
    v6 = a5;
    printf("%d\n", v1);
    printf("%d\n", v2);
    printf("%d\n", v3);
    printf("%d\n", v4);
    printf("%d\n", v5);
    printf("%d\n", v6);
    return 0;
}
