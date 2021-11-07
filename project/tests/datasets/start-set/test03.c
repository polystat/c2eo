//clang -Xclang -ast-dump -fsyntax-only test03.c

int a = 4;
int b = 3;
int c;
int d;
int e;
int f;
int g;
int h;
int i;

 
int main(int argc, char **argv) {
    c = a + b;
    d = a - b;
    e = a * b;
    f = a / b;
    g = a % b;
    h = -a;
    i = a;
    a; b; c; d; e; f; g; h; i;

}
