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
            printf(" %10.3e", a[i * m + j]);
        }
        printf("\n");
	}
}

double abs_(double x){
    if(x < 0){
        return -x;
    }
    return x;
}

void del_col(double* a, int m, int n, int num){
    int i, j;
    double* pos = a;
    double* addr;
    for(i = 0; i < m; ++i){
        addr = a + i * n;
        for(j = 0; j < n; ++j){
            if(j != num){
                *pos = addr[j];
                ++pos;
            }
        }
    }
}



int task01(double* a, int m, int n){
    int i, j;
    int num = 0;
    int mn = abs_(a[0]);
    double tmp = 0;
    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
            if(num != j && (tmp = abs_(a[i * n + j])) < mn){
                mn = tmp;
                num = j;
            }
        }
    }
    del_col(a, m, n, num);
    return(num + 1);
}

void del_crs(double* a, int m, int n, int num_col, int num_row){
    int i, j;
    double* pos = a;
    double* addr;

    for(i = 0; i < num_row; ++i){
        addr = a + i * n;
        for(j = 0; j < n; ++j){
            if(j != num_col){
                *pos = addr[j];
                ++pos;
            }
        }
    }

    for(i = num_row + 1; i < m; ++i){
        addr = a + i * n;
        for(j = 0; j < n; ++j){
            if(j != num_col){
                *pos = addr[j];
                ++pos;
            }
        }
    }
}

int task02(double* a, int m, int n){
    int i, j;
    int num = 0;
    int mn = abs_(a[0]);
    double tmp = 0;
    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
            if(num != j && (tmp = abs_(a[i * n + j])) < mn){
                mn = tmp;
                num = j;
            }
        }
    }
    del_crs(a, m, n, num, num);
    return(num * n + num);
}

int task03(double* a, int m, int n){
    int i, j;
    int num = 0;
    double tmp;
    double mn = 0;
    for(i = 0; i < m; ++i){
        mn += abs_(a[i * n]);
    }
    for(j = 1; j < n; ++j){
        tmp = 0;
        for(i = 0; i < m; ++i){
            tmp += abs_(a[i * n + j]);
        }
        if(tmp < mn){
            mn = tmp;
            num = j;
        }
    }
    del_col(a, m, n, num);
    return (num + 1);
}

int task04(double* a, int m, int n){
    int i,j;
    int num_col = 0, num_row = 0;
    double mn_col = 0, mn_row = 0;
    double tmp;
    for(i = 0; i < m; ++i){
        mn_col += abs_(a[i * n]);
    }
    for(j = 1; j < n; ++j){
        tmp = 0;
        for(i = 0; i < m; ++i){
            tmp += abs_(a[i * n + j]);
        }
        if(tmp < mn_col){
            mn_col = tmp;
            num_col = j;
        }
    }
    for(j = 0; j < n; ++j){
        mn_row += abs_(a[j]);
    }
    for(i = 1; i < m; ++i){
        tmp = 0;
        for(j = 0; j < n; ++j){
            tmp += abs_(a[i * n + j]);
        }
        if(tmp < mn_row){
            mn_row = tmp;
            num_row = i;
        }
    }
    del_crs(a, m, n, num_col, num_row);
    return(num_row * n + num_col);
}

int task05(double* a, int m, int n){
    int i, j;
    int num = 0;
    double mm = -1;
    double tmp;
    for(j = 0; j < n; ++j){
        tmp = 0;
        for(i = 0; i < j; ++i){
            tmp += abs_(a[i * m + j]);
        }

        for(i = j + 1; i < m; ++i){
            tmp += abs_(a[i * m + j]);
        }
        if(tmp > mm){
            mm = tmp;
            num = j;
        }
    }
    del_col(a, m, n, num);
    return(num + 1);
}

int task06(double* a, int m, int n){
    int i, j;
    int num = 0;
    double mm = -1;
    double tmp;
    double el;
    int k = (n > m ? m : n);
    for(j = 0; j < k; ++j){
        tmp = 0;
        el = a[j * n + j];
        for(i = 0; i < j; ++i){
            tmp += abs_(a[i * m + j] - el);
        }

        for(i = j + 1; i < m; ++i){
            tmp += abs_(a[i * m + j] - el);
        }
        //printf("j = %d tmp = %lf\n", j, tmp);
        if(tmp > mm){
            mm = tmp;
            num = j;
        }
    }
    del_col(a, m, n, num);
    return(num + 1);
}

int task07(double* a, int m, int n){
    int i, j;
    int num = 0;
    double msum = 0;
    double tmp, tmp_sqr = 0, tmp_s = 0;
    double tmp_sum;
    for(i = 0; i < m; ++i){
        tmp = a[i * m];
        tmp_sqr += tmp * tmp;
        tmp_s += tmp;
    }
    msum = tmp_sqr - (tmp_s * tmp_s) / m;
    for(j = 1; j < n; ++j){
        tmp_s = 0;
        tmp_sqr = 0;
        for(i = 0; i < m; ++i){
        tmp = a[i * m];
        tmp_sqr += tmp * tmp;
        tmp_s += tmp;
        }
        tmp_sum = tmp_sqr - (tmp_s * tmp_s) / m;
        if(tmp_sum < msum){
            msum = tmp_sum;
            num = j;
        }
    }
    del_col(a, m, n, num);
    return (num + 1);
}

int task08(double* a, int m, int n){
    int i, j;
    int num_col = 0, num_row = 0;
    double tmp_row, tmp_col;
    double max_col = -1;
    double max_row = -1;
    for(j = 0; j < n; ++j){
        tmp_col = 0;
        for(i = 0; i < j; ++i){
            tmp_col += abs_(a[i * m + j]);
        }

        for(i = j + 1; i < m; ++i){
            tmp_col += abs_(a[i * m + j]);
        }
        if(tmp_col > max_col){
            max_col = tmp_col;
            num_col = j;
        }
    }

    for(i = 0; i < m; ++i){
        tmp_row = 0;
        for(j = 0; j < i; ++j){
            tmp_row += abs_(a[i * m + j]);
        }
        
        for(j = i + 1; j < n; ++j){
            tmp_row += abs_(a[i * m + j]);
        }
        if(tmp_row > max_row){
            max_row = tmp_row;
            num_row = i;
        }
    }

    del_crs(a, m, n, num_col, num_row);
    return(num_row * m + num_col);
}

int task10(double* a, int m, int n){
    int i, j;
    int num_col = 0, num_row = 0;
    double msum = 0;
    double tmp, tmp_sqr = 0, tmp_s = 0;
    double tmp_sum;
    for(i = 0; i < m; ++i){
        tmp = a[i * m];
        tmp_sqr += tmp * tmp;
        tmp_s += tmp;
    }
    msum = tmp_sqr - (tmp_s * tmp_s) / m;
    for(j = 1; j < n; ++j){
        tmp_s = 0;
        tmp_sqr = 0;
        for(i = 0; i < m; ++i){
            tmp = a[i * m];
            tmp_sqr += tmp * tmp;
            tmp_s += tmp;
        }
        tmp_sum = tmp_sqr - (tmp_s * tmp_s) / m;
        if(tmp_sum < msum){
            msum = tmp_sum;
            num_col = j;
        }
    }
    tmp_sqr = 0; 
    tmp_s = 0;
    msum = 0;
    for(j = 0; j < n; ++j){
        tmp = a[j];
        tmp_sqr += tmp * tmp;
        tmp_s += tmp;
    }
    msum = tmp_sqr - (tmp_s * tmp_s) / n;
    for(i = 0; i < m; ++i){
        tmp_s = 0;
        tmp_sqr = 0;
        for(j = 0; j < n; ++j){
            tmp = a[i * m + j];
            tmp_sqr += tmp * tmp;
            tmp_s += tmp;
        }
        tmp_sum = tmp_sqr - (tmp_s * tmp_s) / n;
        if(tmp_sum < msum){
            msum = tmp_sum;
            num_row = i;
        }
    }
    del_crs(a, m, n, num_col, num_row);
    return(num_row * m + num_col);
}