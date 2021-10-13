// test000.h
//clang -Xclang -ast-dump -fsyntax-only main.cpp

int intVar01;
static int intVar00;

void func(int x)
{
    static int intVar02 = 5;
}
