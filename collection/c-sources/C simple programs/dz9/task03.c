#include <time.h>

#include "func.h"

#define TASK_NUM 3

int main(int argc, char *argv[]){
    double* x = 0;
    double* y = 0;
    int read_res = 0;
    int n = 0;
    char* name = 0;
    double t = 0, x0 = 0;
    double res = 0;

    if (!(argc == 4 && sscanf(argv[1], "%lf", &x0) == 1 && sscanf(argv[2], "%d", &n) == 1))
    {
        printf("Usage: %s x0 n name\n", argv[0]);
        return 1;
    }
    name = argv[3];

    x = (double*)malloc(n * sizeof(double));
    if (!x)
    {
        printf("Cannot allocate %d elements\n", n);
        return 2;
    }
    y = (double*)malloc(n * sizeof(double));
    if (!y)
    {
        printf("Cannot allocate %d elements\n", n);
        free(x);
        return 3;
    }
    read_res = read_arrays(x, y, n, name);
    if (read_res != ERROR_SUCCESS)
    {
        switch(read_res)
        {
            case ERROR_OPEN: 
                printf("Cannot open %s\n", name);
                break;
            case ERROR_READ: 
                printf("Cannot read %s\n", name);
                break;
            case ERROR_EQUAL: 
                printf("x[i] = x[j] in %s\n", name);
                break;
            default: 
                printf("Unknown error in %s\n", name);
        }
        free(x);
        free(y);
        return 4;
    }

    t = clock();
    res = task03(x0, n, x, y);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf("%s : Task = %d Result = %e Elapsed = %.2f\n", argv[0], TASK_NUM, res, t);

    free(x);
    free(y);
    return 0;
}
