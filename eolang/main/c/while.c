#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int i = 0;
    while (i < a)
    {
        printf("while[%d] ", i);
        i++;
    }
}