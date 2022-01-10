//clang -Xclang -ast-dump -fsyntax-only test01.c
//#include "stdio.h"
struct a{
    struct b{
        int a;
    } q;
};
int main() {
    struct a1{
        struct b1{
            int a;
        } q;
    };
//    printf("%d\n", r);
    return 0;
}