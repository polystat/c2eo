//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c;
int d;
double e;
_Bool b;
char a;

long long *pc;
int *pd;
long long *pe;

int main() {
    c = 255;
    pc = &c;
    pd = pc;
    pc++;
    ++pc;
    pc--;
    --pc;
    pc = pe;
    pc = (long long *)pd + 5;
    pc = (long long *)pd - 5;
    pc = (long long *)pd + c;
    pc = (long long *)pd - c;
    pc += c;
    pc += a;
    pc -= d;
    pc -= b;
    pc = (long long *)c;
    pc = (long long *)10;
    pc = (long long *)(long long)e;
    pc = (long long *)pd + c;
    d = pc == pe;
    b = pc != pe;
    d = (pc > pe);
    pc = (void*)'a';
    pc = (void*)pd + c;
    b = pc && pd;
    b = pc || pd;
    b = !pe;

    pc = c;
    pc = 'a';
    pc = 10;
    d = pc < (void*)pd;
    d = pc < pd;
    pc = pd + c;
    pc = !pe;

    printf("c = %lld\n", c);
    printf("*pc = %lld\n", *pc);
    printf("d = %d\n", d);
    printf("*pd = %d\n", *pd);
    printf("*pe = %lld\n", *pe);

    return 0;
}
