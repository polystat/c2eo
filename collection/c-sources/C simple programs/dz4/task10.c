#include <time.h>
#include "matrix.h"

#define TASK_NUM 10

int main(int argc, char *argv[])
{
    double* a;
    double* b;
    double* c;
    int k = 0;
    int m = 0 ,n = 0, p = 0, ka = 0, kb = 0 ,read_res = 0;
    char* namea = 0;
    char* nameb = 0;
    double t = 0;	

	if (argc == 7){
		if (!(sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &k) && sscanf(argv[4], "%d", &p) == 1 
		&& sscanf(argv[5], "%d", &ka) == 1 && sscanf(argv[6], "%d", &kb) == 1
		&& ka > 0 && ka <= 4 && kb > 0 && kb <= 4 )){
			printf ("Usage : %s n m p ka [filea] kb [fileb]\n", argv[0]);
			return 1;
		}
    }

	if (argc == 8){
		if (sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &k) && sscanf(argv[4], "%d", &p) == 1
		&& sscanf(argv[5], "%d", &ka) == 1	&& sscanf(argv[7], "%d", &kb) == 1
		&& ka == 0 && kb > 0 && kb <= 4 ){
			namea = argv[6];
		} else if ((sscanf (argv[1], "%d", &n) == 1) && sscanf(argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &k) && sscanf(argv[4], "%d", &p) == 1 
		&& sscanf(argv[5], "%d", &ka) == 1 && sscanf(argv[6], "%d", &kb) == 1
		&& kb == 0 && ka > 0 && ka <= 4){
				nameb = argv[7];
		} else {
			printf ("Usage : %s n m p ka [filea] kb [fileb]\n", argv[0]);
			return 2;
		}
	}
		
	if (argc == 9){
		if (!( sscanf (argv[1], "%d", &n) == 1 && sscanf (argv[2], "%d", &m) == 1 
        && sscanf (argv[3], "%d", &k) && sscanf(argv[4], "%d", &p) == 1 
		&& sscanf (argv[5], "%d", &ka) == 1 && sscanf (argv[7], "%d", &kb) == 1
		&& ka == 0 && kb == 0 )){
			printf ("Usage : %s n m p ka [filea] kb [fileb]\n", argv[0]);
			return 3;
		}
		namea = argv[6];
		nameb = argv[8];
	}

    a = (double*)malloc(n * m * sizeof(double));
    if(!a){
        printf("Cannot allocate %d elements\n", n * m);
        return 4;
    }
    b = (double*)malloc(m * k * sizeof(double));
    if(!b){
        free(a);
        printf("Cannot allocate %d elements\n", m * k);
        return 5;
    }
    c = (double*)malloc(n * k * sizeof(double));
    if(!c){
        free(a);
        free(b);
        printf("Cannot allocate %d elements\n", n * k);
        return 6;
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
            free(a);
            free(b);
            free(c);
            return 7;
        }
    } else {
        init_matrix(a, n, m, ka);
    }

    if(nameb){
        read_res = read_matrix(b, m, k, nameb);
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
            free(c);
            return 7;
        }
    } else {
        init_matrix(b, m, k, kb);
    }
    
    printf("Matrix 1 :\n");
    printf("______________\n");
    print_matrix(a, n, m, p);
    printf("______________\n");

    printf("Matrix 2 :\n");
    printf("______________\n");
    print_matrix(b, m, k, p);
    printf("______________\n");

    t = clock();
    task10(a, b, c, n, m, k);
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf("New matrix :\n");
    printf("______________\n");
    print_matrix(c, n, k, p);
    printf("______________\n");

    printf ("%s : Task = %d Elapsed = %.2lf", argv[0], TASK_NUM, t);
    free(a);
    free(b);
    free(c);
    return 0;
}

