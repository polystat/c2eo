// test000.h
//clang -Xclang -ast-dump -fsyntax-only main.cpp

int unsigned intVar01;
int intVar02 = 3 + 5;
const int x = 0x10;
//float y = 3.14;
double y2 = 3.14;      // не конвертирует
long long t = 100ll;
_Bool boolVal01;
_Bool boolVal02 = 111-11-100;
static int a = 666;
unsigned char c;
char ch = 'a';

void f() {
    int xxx;
    auto int yyy;
    static int zzz;
}

struct S {
    unsigned int ssss;
    int tttt;
    //static int tttt;
};
