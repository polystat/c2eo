#include <time.h>
#include "matrix.h"

#define TASK_NUM 8

int main(int argc, char *argv[])
{
    double* a;
    int m = 0 ,n = 0, p = 0, k = 0, read_res = 0;
    int i = 0, j = 0;
    char* name = 0;
    double t = 0;
    double g = 0;
    if (!((argc == 8 || argc == 9) && sscanf(argv[1], "%d", &i) == 1 && sscanf(argv[2], "%d", &j) == 1 
	&& sscanf(argv[4], "%d", &n) == 1 && sscanf(argv[5], "%d", &m) == 1 && sscanf(argv[6], "%d", &p) == 1 
    && sscanf(argv[7], "%d", &k) == 1 && sscanf(argv[3], "%lf", &g) == 1  
    && k >= 0 && k <= 4 && i > 0 && i <= n && j > 0 && j <= n)){
        printf("Usage : %s n p k [name]\n", argv[0]);
        return 1;
    }
    if(k != 0 && argc == 9){
        printf("Wrong formula\n");
        return 2;
    }
    if(k == 0 && argc == 8){
        printf("Wrong number of arguments\n");
        return 3;
    }
    if (argc == 9){
        name = argv[8];
    }
    a = (double*)malloc(n * m * sizeof(double));
    if(!a){
        printf("Cannot allocate %d elements\n", n * n);
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
    task08(a, m, i, j, g);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf("New matrix :\n");
    printf("______________\n");
    print_matrix(a, n, m, p);
    printf("______________\n");

    printf ("%s : Task = %d Elapsed = %.2lf", argv[0], TASK_NUM, t);
    free(a);
    return 0;
}

