#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    a = atoi(argv[1]);
    printf("switch[%d] = ", a);
    switch (a) {
     case 0:
       printf("0");
       break;
     case 1:
       printf("1");
       break;
     case 5:
       printf("5");
       break;
     default:
       printf("default");
       break;
   }
}