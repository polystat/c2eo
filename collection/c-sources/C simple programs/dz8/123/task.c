#include <time.h>

#include "matrix.h"
#include "solve.h"

#define TASK_NUM 1

int main(int argc, char *argv[])
{
    double* a = 0;
    double* b = 0;
    double* x = 0;
    int n = 0, p = 0, k = 0, read_res = 0, res = 0;
    double r1, r2;
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
    b = (double*)malloc(n * sizeof(double));
    if(!b){
        free(a);
        printf("Cannot allocate %d elements\n", n);
        return 4;
    }
    x = (double*)malloc(n * sizeof(double));
    if(!x){
        free(a);
        free(b);
        printf("Cannot allocate %d elements\n", n);
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
            free(b);
            free(x);
            return 5;
        }
    } else {
        init_matrix(a, n, n, k);
    }
    make_b(a, b, n);
    
    printf("Matrix :\n");
    print_matrix(a, n, n, p);
    printf("______________\n");
    printf("Vector b:\n");
    print_matrix(b, 1 , n, p);
    printf("______________\n");

    t = clock();
    res = solve(a, b, x, n);
    t = (clock() - t) / CLOCKS_PER_SEC;

    if(res == 1){
        printf("System can't be solved\n");
        free(a);
        free(b);
        free(x);
        return 6;
    }
    printf("Vector x:\n");
    print_matrix(x, 1 , n, p);
    printf("______________\n");
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
            free(b);
            free(x);
            return 5;
        }
    } else {
        init_matrix(a, n, n, k);
    }
    make_b(a, b, n);

    r1 = f_r1(a, b, x, n);
    r2 = f_r2(x, n);


    printf ("%s : Task = %d Res1 = %e Res2 = %e Elapsed = %.2f\n", argv[0], 9, r1, r2, t);
    free(a);
    free(b);
    free(x);
    return 0;
}