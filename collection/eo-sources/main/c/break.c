#include <stdio.h>
#include <stdlib.h>

int breakOrReturn(int a)
{
    while (1)
    {
        if ((a % 5) == 0)
        {
            break;
        }

        if ((a % 3) == 0)
        {
            return 1;
        }

        a++;
    }

    printf("We broke out of the loop\n");
    return 0;
}


int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = breakOrReturn(a);
    printf("breakOrReturn = %d", result);
}