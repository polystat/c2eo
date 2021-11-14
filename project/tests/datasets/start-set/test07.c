//clang -Xclang -ast-dump -fsyntax-only test07.c

//#include <stdio.h>

int c;
int a = 2;
int b = 3;

void function_void(int x, int y){
    c = a;
}

int func_int(){
    int ret = 10;
    return ret;
}

int main() {
    function_void(a, b);
//    c = func_int();
    c;
}
