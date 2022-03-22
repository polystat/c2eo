#include "stdio.h"
_Bool var = 1;
_Bool v = 0;
int main() {
    printf("%d\n", v);
    v;
    printf("%d\n", var);
    var;
    return 0;
}
