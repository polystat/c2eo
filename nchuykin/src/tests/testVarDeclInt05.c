// test000.h
//clang -Xclang -ast-dump -fsyntax-only main.cpp

int intVar01;
static int intVar00;

int func(int x)
{
    static int intVar02;
}
