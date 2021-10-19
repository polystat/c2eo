#include <time.h>
#include "matrix.h"

#define TASK_NUM 1

int main(int argc, char *argv[])
{
    double* a;
    int n = 0, p = 0, k = 0, read_res = 0, result = 0;
    char* name = 0;
    double t = 0;
    if(!((argc != 4 || argc != 5) && (sscanf(argv[1], "%d", &n) == 1)
    && (sscanf(argv[2], "%d", &p) == 1) && (sscanf(argv[3], "%d", &k) == 1)
    && k >= 0 && k <= 4)){
        printf("Usage : %s n p k [name]\n", argv[0]);
        return 1;
    }
    if(k != 0 && argc == 5){
        printf("Wrong formula\n");
        return 2;
    }
    if(k == 0 && argc == 4){
        printf("Wrong number of arguments\n");
        return 3;
    }
    if (argc == 5){
        name = argv[4];
    }
    a = (double*)malloc(n * n * sizeof(double));
    if(!a){
        printf("Cannot allocate %d elements\n", n * n);
        return 4;
    }
    if(name){
        read_res = read_matrix(a, n, n, name);
        if(read_res != READ_SUCCESS){
            switch(read_res){
                case READ_ERR_OPEN:
                    printf("Cannot open file %s\n", name);
                    break;
                case READ_ERR_READ:
                    printf("Cannot read file %s\n", name);
                    break;
                default:
                    printf("Unknown error\n");
            }
            free(a);
            return 5;
        }
    } else {
        init_matrix(a, n, n, k);
    }
    
    printf("Matrix :\n");
    printf("______________\n");
    print_matrix(a, n, n, p);
    printf("______________\n");

    t = clock();
    result = task01(a, n);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf ("%s : Task = %d Elapsed = %.2lf Result = %d\n", argv[0], TASK_NUM, t, result);
    free(a);
    return 0;
}

