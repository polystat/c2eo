#include <time.h>
#include "matrix.h"

#define TASK_NUM 8

int main(int argc, char *argv[]){
    double* a;
    char* name = 0;
    int n = 0, m = 0;
    int p = 0, k = 0;
    int read_res = 0;
    int result = 0;
    double t = 0;
    if (!((argc == 5 || argc == 6) && (sscanf(argv[1], "%d", &n) == 1) 
        && (sscanf(argv[2], "%d", &m) == 1)
        && (sscanf(argv[3], "%d", &p) == 1) 
        && (sscanf(argv[4], "%d", &k) == 1) 
        && k >= 0 && k <= 4)) {
        printf("Usage: %s n m p k [name]\n", argv[0]);
        return 1;
    }
    if(k != 0 && argc == 6){
        printf("Wrong formula\n");
        return 2;
    }
    if(k == 0 && argc == 5){
        printf("Wrong number of arguments\n");
        return 3;
    }
    if (argc == 6){
        name = argv[5];
    }
    a = (double*)malloc(n * m * sizeof(double));
    if(!a){
        printf("Cannot allocate %d elements\n", n * m);
        return 4;
    }
    if(name){
        read_res = read_matrix(a, n, m, name);
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
        init_matrix(a, n, m, k);
    }
    
    printf("Matrix :\n");
    printf("______________\n");
    print_matrix(a, n, m, p);
    printf("______________\n");

    t = clock();
    result = task08(a, n, m);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf("Matrix :\n");
    printf("______________\n");
    print_matrix(a, n - 1, m - 1, p);
    printf("______________\n");

    printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], TASK_NUM, result, t);
    free(a);
    return 0;
}