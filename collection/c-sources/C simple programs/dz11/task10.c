#include <time.h>
#include <stdio.h>

#include "func.h"

#define TASK_NUM 10

int main(int argc, char *argv[]){
    double a = 0, b = 0, e = 0;
    int k = 0;
    double (*f)(double);
    double res = 0;
    double t = 0;
    int count_f = 0;
    double (*funcs[])(double) = {f0, f1, f2, f3, f4, f5, f6};
    if (!( argc == 4  && 
        sscanf(argv[1], "%lf", &a) == 1 &&
        sscanf(argv[2], "%lf", &e) == 1 &&
        sscanf(argv[3], "%d", &k) == 1 
        && e > 0 && k >= 0 && k <=6 )){
            printf("Usage: %s a e k\n", argv[0]);
            return 1;
    }
    f = funcs[k];
    
    t = clock();
    b = task10(f, a, e, &res);
    t = (clock() - t) / CLOCKS_PER_SEC;
    if (b < 0){
        printf("Cannot calculate\n");
        return 2;
    }
    count_f = getCount();

    printf ("%s : Task = %d Res = %e B = %e Count = %d T = %.2f\n", argv[0],TASK_NUM, res, b, count_f, t);
    return 0;
}
