#include <stdio.h>
 
void main() {
    int a = 0x77, b = -20;
    char c = 'F';
    float f = 12.2341524;
    double d = 2e8;
    char* string = "Hello, World!";
 
    printf("%s\n", string);
    printf("a = %d, b = %d\n", a, b);
    printf("a = %u, b = %u\n", a, b);
    printf("a = %x, b = %X\n", a, b);
    printf("dec a = %d, oct a = %o, hex a = %x\n", a, a, a);
    printf("floating point f = %f, exp f = %e\n", f, f);
    printf("double d = %f or %E\n", d, d);
    printf("not all compiler support %a\n", f);
    printf("character c = %c, as number c = %d\n", c, c);
    printf("address of string is %p", string);
}
