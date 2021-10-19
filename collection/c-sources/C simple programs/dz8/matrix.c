#include "matrix.h"

double f(int k, int n, int m, int i, int j){
    switch (k)
        {
            case 1:
                return ((n > m ? n : m) - (i > j ? i : j) + 1);
            case 2:
                return (i > j ? i : j);
            case 3:
                return (i > j ? i - j : j - i); 
            case 4:
                return (1.0 / (i + j - 1));
        }
    return 0;
}

void init_matrix (double* a, int n, int m, int k){
    int i = 0, j = 0;
    for(i = 0; i < n; ++i){
        for(j = 0; j < m; ++j){
            a[i * m + j] = f(k, n, m, i + 1, j + 1);
        }
    }
}

int read_matrix(double* a, int n, int m, const char* name){
    FILE* fp;
    int i = 0, j = 0;
    if(!(fp = fopen(name, "rt"))){
        return READ_ERR_OPEN;
    }
    for(i = 0; i < n; ++i){
        for(j = 0; j < m; ++j){
            if(fscanf(fp, "%lf", a + i * m + j) != 1){
                fclose(fp);
                return READ_ERR_READ;
            }
        }
    }
    fclose(fp);
    return READ_SUCCESS;
}

void print_matrix (double* a, int n, int m, int p)
{
	int n1 = (n > p ? p : n);
	int m1 = (m > p ? p : m);
	int i = 0, j = 0;
	for (i = 0; i < n1; ++i){
        for(j = 0; j < m1; ++j){
            printf("%10.2e", a[i * m + j]);
        }
        printf("\n");
	}
}

double f_r1(double* a, double* b, double* x, int n){
    int i,j;
    double tmp;
    double sum1 = 0, sum2 = 0;
    double* addr;
    for(i = 0; i < n; ++i){
        tmp = 0;
        addr = a + i * n;
        for(j = 0; j < n; ++j){
            tmp += addr[j] * x[j];
        }
        tmp -= b[i];
        sum1 += fabs(tmp);
        sum2 += fabs(b[i]);
    }
    return (sum1 / sum2);
}

double f_r2(double* x, int n){
    int i;
    double tmp = 0.0;
    for(i = 0; i < n; ++i){
        tmp += fabs(x[i] - ((i + 1) & 1));
    }
    return tmp;
}

void make_b(double* a, double* b, int n){
    int i, j;
    double* addr;
    double tmp;
    for(i = 0; i < n; ++i){
        addr = a + i * n;
        tmp = 0;
        for(j = 0; j < n; j += 2){
            tmp += addr[j];
        }
        b[i] = tmp;
    }
}

