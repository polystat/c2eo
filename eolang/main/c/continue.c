#include <stdio.h>
#include <stdlib.h>

int collatzProblem(int number)
{
    while (number > 1)
    {
        if ((number % 2) == 0 )
        {
            number = number / 2;
            continue;
        }

        number = (number * 3) + 1;
    }

    return number;
}


int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = collatzProblem(a);
    printf("collatzProblem(%d) = %d", a, result);
}