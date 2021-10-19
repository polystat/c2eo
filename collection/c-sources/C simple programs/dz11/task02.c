#include <time.h>

#include "func.h"

#define TASK_NUM 2

int main(int argc, char *argv[]){
    int count_f = 0;
    int k = 0;
    double x = 0, h = 0;
    double t = 0, res = 0;
    double(*f)(double);
    double (*funcs[])(double) = {f0, f1, f2, f3, f4, f5, f6};
    if (!(argc == 4 
        && sscanf(argv[1], "%lf", &x) == 1 
        && sscanf(argv[2], "%lf", &h) == 1
        && sscanf(argv[3], "%d", &k) == 1
        && k >= 0 && k <= 6)){
            printf("Usage: %s x h k\n", argv[0]);
            return 1;
    }
    f = funcs[k];

    t = clock();
    res = task02(f, x, h);
    t = (clock() - t) / CLOCKS_PER_SEC;   
    count_f = getCount();

    printf ("%s : Task = %d Res = %e Count = %d T = %.2f\n", argv[0], TASK_NUM, res, count_f, t);
    return 0;
}
