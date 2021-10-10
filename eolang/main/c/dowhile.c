#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int i = 0;
    do
    {
        printf("do while[%d] ", i);
        i++;
    } while (i < a);
}