#include <time.h>
#include "matrix.h"

#define TASK_NUM 4

int main(int argc, char *argv[]){
    double* a;
    double* b;
    double* x;
    char* namea = 0;
    char* nameb = 0;
    char* namex = 0;
    int n = 0, m = 0;
    int p = 0, ka = 0, kb = 0, kx = 0;
    int read_res = 0;
    double result = 0;
    double t = 0;
    if(argc != 7 && argc != 8 && argc != 9 && argc != 10) {
        printf("2131Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
        return 1;
    }
    if (!(sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &m) == 1 && sscanf(argv[3], "%d", &p) == 1)){
        printf("123Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
        return 1;
    }
    if(argc == 7) {
        if (!(sscanf(argv[4], "%d", &ka) == 1 && ka > 0 && ka <= 4
                && sscanf(argv[5], "%d", &kb) == 1 && kb > 0 && kb <= 4
                && sscanf(argv[6], "%d", &kx) == 1 && kx > 0 && kx <= 4)) {
                    printf("231Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
                    return 1;
                }
    } else if(argc == 8){
        if (sscanf(argv[4], "%d", &ka) == 1 && ka == 0
                && sscanf(argv[6], "%d", &kb) == 1 && kb > 0 && kb <= 4
                && sscanf(argv[7], "%d", &kx) == 1 && kx > 0 && kx <= 4) {
                    namea = argv[5];
        } else if(sscanf(argv[4], "%d", &ka) == 1 && ka > 0 && ka <= 4
                && sscanf(argv[5], "%d", &kb) == 1 && kb == 0
                && sscanf(argv[7], "%d", &kx) == 1 && kx > 0 && kx <= 4){
                    nameb = argv[6];
        } else if(sscanf(argv[4], "%d", &ka) == 1 && ka > 0 && ka <= 4
                && sscanf(argv[5], "%d", &kb) == 1 && kb > 0 && kb <= 4
                && sscanf(argv[6], "%d", &kx) == 1 && kx == 0){
                    namex = argv[7];
        } else {
            printf("Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
            return 1;
        }
    } else if(argc == 9){
            if (sscanf(argv[4], "%d", &ka) == 1 && ka == 0
                && sscanf(argv[6], "%d", &kb) == 1 && kb == 0
                && sscanf(argv[8], "%d", &kx) == 1 && kx > 0 && kx <= 4) {
                    namea = argv[5];
                    nameb = argv[7];
        } else if (sscanf(argv[4], "%d", &ka) == 1 && ka == 0
                && sscanf(argv[6], "%d", &kb) == 1 && kb > 0 && kb <= 4
                && sscanf(argv[7], "%d", &kx) == 1 && kx == 0) {
                    namea = argv[5];
                    namex = argv[8];
        } else if (sscanf(argv[4], "%d", &ka) == 1 && ka > 0 && ka <= 4
                && sscanf(argv[5], "%d", &kb) == 1 && kb == 0
                && sscanf(argv[7], "%d", &kx) == 1 && kx == 0) {
                    nameb = argv[6];
                    namex = argv[8];
        } else {
            printf("Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
            return 1;
        }
    } else if(argc == 10) {
        if (sscanf(argv[4], "%d", &ka) == 1 && ka > 0 && ka <= 4
                && sscanf(argv[6], "%d", &kb) == 1 && kb == 0
                && sscanf(argv[8], "%d", &kx) == 1 && kx == 0) {
                    namea = argv[5];
                    nameb = argv[7];
                    namex = argv[9];
        } else {
            printf("Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
            return 1;
        }
    } else {
        printf("Usage: %s n m p ka [namea] kb [nameb] kx [namex]\n", argv[0]);
        return 1;
    }

    a = (double*)malloc(n * m * sizeof(double));
    if(!a){
        printf("Cannot allocate %d elements\n", n * m);
        return 2;
    }
    b = (double*)malloc(n * sizeof(double));
    if(!b){
        printf("Cannot allocate %d elements\n", n);
        free(a);
        return 2;
    }
    x = (double*)malloc(m * sizeof(double));
    if(!x){
        printf("Cannot allocate %d elements\n", m);
        free(a);
        free(b);
        return 2;
    }
    if(namea){
        read_res = read_matrix(a, n, m, namea);
        if(read_res != READ_SUCCESS){
            switch(read_res){
                case READ_ERR_OPEN:
                    printf("Cannot open file %s\n", namea);
                    break;
                case READ_ERR_READ:
                    printf("Cannot read file %s\n", namea);
                    break;
                default:
                    printf("Unknown error\n");
            }
            free(b);
            free(x);
            free(a);
            return 3;
        }
    } else {
        init_matrix(a, n, m, ka);
    }

    if(nameb){
        read_res = read_matrix(b, n, 1, nameb);
        if(read_res != READ_SUCCESS){
            switch(read_res){
                case READ_ERR_OPEN:
                    printf("Cannot open file %s\n", nameb);
                    break;
                case READ_ERR_READ:
                    printf("Cannot read file %s\n", nameb);
                    break;
                default:
                    printf("Unknown error\n");
            }
            free(b);
            free(x);
            free(a);
            return 3;
        }
    } else {
        init_matrix(b, n, 1, kb);
    }

    if(namex){
        read_res = read_matrix(x, m, 1, namex);
        if(read_res != READ_SUCCESS){
            switch(read_res){
                case READ_ERR_OPEN:
                    printf("Cannot open file %s\n", namex);
                    break;
                case READ_ERR_READ:
                    printf("Cannot read file %s\n", namex);
                    break;
                default:
                    printf("Unknown error\n");
            }
            free(b);
            free(x);
            free(a);
            return 3;
        }
    } else {
        init_matrix(x, m, 1, kx);
    }
    
    
    printf("Matrix A :\n");
    printf("______________\n");
    print_matrix(a, n, m, p);
    printf("______________\n");
    printf("Matrix B :\n");
    printf("______________\n");
    print_matrix(b, n, 1, p);
    printf("______________\n");
    printf("Matrix X :\n");
    printf("______________\n");
    print_matrix(x, m, 1, p);
    printf("______________\n");

    t = clock();
    result = task04(a, b, x, n, m);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf ("%s : Task = %d Result = %lf Elapsed = %.2f\n", argv[0], TASK_NUM, result, t);
    free(a);
    free(b);
    free(x);
    return 0;
}