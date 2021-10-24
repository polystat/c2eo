//clang -Xclang -ast-dump -fsyntax-only test04.c

_Bool a = 0;
_Bool b = 1;
_Bool c;
_Bool d;
_Bool e;
_Bool f;
_Bool g;
_Bool h;
_Bool i;
_Bool j;
int x = -3;
int y = 3;
int zero = 0;


int main() {
    c = a == b;
    d = a != b;
    e = a < b;
    f = a <= b;
    g = a > b;
    h = a >= b;
    i = (x<zero)*-x +(x>=zero)*x;
    j = (y<zero)*-y +(y>=zero)*y;
    //return 0;
}