#include <time.h>
#include "matrix.h"

#define TASK_NUM 2

int main(int argc, char *argv[])
{
    double* a;
    double* x0;
    double* x;
    double* b;
    double tau = 0;
    double r1 = 0, r2 = 0;
    int m = 0, n = 0, p = 0, ka = 0, kb = 0 , read_res = 0;
    char* namea = 0;
    char* nameb = 0;
    double t = 0;	
    if(argc != 7 && argc != 8 && argc != 9){
        printf ("Usage : %s tau m n p ka [filea] kb [fileb]\n", argv[0]);
		return 1;
    }

	if (argc == 7){
		if (!(sscanf(argv[1], "%lf", &tau) == 1 && sscanf(argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &n) && sscanf(argv[4], "%d", &p) == 1 
		&& sscanf(argv[5], "%d", &ka) == 1 && sscanf(argv[6], "%d", &kb) == 1
		&& ka > 0 && ka <= 4 && kb > 0 && kb <= 4 )){
			printf ("Usage : %s tau m n p ka [filea] kb [fileb]\n", argv[0]);
			return 1;
		}
    }

	if (argc == 8){
		if (sscanf(argv[1], "%lf", &tau) == 1 && sscanf(argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &n) && sscanf(argv[4], "%d", &p) == 1
		&& sscanf(argv[5], "%d", &ka) == 1	&& sscanf(argv[7], "%d", &kb) == 1
		&& ka == 0 && kb > 0 && kb <= 4 ){
			namea = argv[6];
		} else if ((sscanf (argv[1], "%lf", &tau) == 1) && sscanf(argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &n) && sscanf(argv[4], "%d", &p) == 1 
		&& sscanf(argv[5], "%d", &ka) == 1 && sscanf(argv[6], "%d", &kb) == 1
		&& kb == 0 && ka > 0 && ka <= 4){
				nameb = argv[7];
		} else {
			printf ("Usage : %s tau m n p ka [filea] kb [fileb]\n", argv[0]);
			return 2;
		}
	}
		
	if (argc == 9){
		if (!( sscanf (argv[1], "%lf", &tau) == 1 && sscanf (argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &n) && sscanf(argv[4], "%d", &p) == 1 
		&& sscanf (argv[5], "%d", &ka) == 1 && sscanf (argv[7], "%d", &kb) == 1
		&& ka == 0 && kb == 0 )){
			printf ("Usage : %s tau m n p ka [filea] kb [fileb]\n", argv[0]);
			return 3;
		}
		namea = argv[6];
		nameb = argv[8];
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
        return 5;
    }
    x0 = (double*)malloc(n * sizeof(double));
    if(!x0){
        free(a);
        free(b);
        printf("Cannot allocate %d elements\n", n);
        return 6;
    }
    x = (double*)malloc(n * sizeof(double));
    if(!x){
        free(a);
        free(b);
        free(x0);
        printf("Cannot allocate %d elements\n", n);
        return 7;
    }
    if(namea){
        read_res = read_matrix(a, n, n, namea);
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
            free(a);
            free(b);
            free(x0);
            free(x);
            return 8;
        }
    } else {
        init_matrix(a, n, n, ka);
    }

    if(nameb){
        read_res = read_matrix(x0, n, 1, nameb);
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
            free(a);
            free(b);
            free(x0);
            free(x);
            return 9;
        }
    } else {
        init_matrix(x0, n, 1, kb);
    }

    make_b(a, b, n);
    
    printf("Matrix 1 :\n");
    printf("______________\n");
    print_matrix(a, n, n, p);
    printf("______________\n");

    printf("Vector x0 :\n");
    printf("______________\n");
    print_matrix(x0, 1, n, p);
    printf("______________\n");

    printf("Vector b :\n");
    printf("______________\n");
    print_matrix(b, 1, n, p);
    printf("______________\n");

    t = clock();
    task02(a, b, x, x0, n, m, tau);
    r1 = f_r1(a, b, x, n);
    r2 = f_r2(x, n);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf("Vector x :\n");
    printf("______________\n");
    print_matrix(x, 1, n, p);
    printf("______________\n");

    printf ("%s : Task = %d Res1 = %e Res2 = %e Elapsed = %.2f\n", argv[0], TASK_NUM, r1, r2, t);
    free(a);
    free(b);
    free(x0);
    free(x);
    return 0;
}