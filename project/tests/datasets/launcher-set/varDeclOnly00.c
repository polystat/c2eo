// varDeclOnly.c
//clang -Xclang -ast-dump -fsyntax-only main.cpp

int unsigned intVar01;
int intVar02 = 3 + 5;
const int x = 0x10;
double y2 = 3.14;      // не конвертирует
long long t = 100ll;
_Bool boolVal01;
_Bool boolVal02 = 111-11-100;
static int a = 666;
unsigned char c;
char ch = 'a';

static int stat01;
