#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "solve.h"
#include "matrix.h"

const double epsilon = 10e-17;

double norm(double *b, int n){
    int i;
    double tmp = 0.0;
    for (i = 0; i < n; ++i)
        tmp += fabs(b[i]);
    return tmp;
}

int solve(double *a, double *b, double *x, int n){
    int index = 0;
    double tmp;
    double  max_el = 0;
    double eps = norm(b, n) * epsilon;

    for (int k = 0; k < n; k ++) {
        max_el = a[k * n + k];
        index = k;
        for(int i = k + 1; i < n; i++){
            if(fabs(a[i * n + k]) > fabs(max_el)){
                max_el = a[i * n + k];
                index = i;
            }
        }
        if (fabs(max_el) < eps){
            return 1;
        }

        for(int j = 0 ; j < n; ++j){
            tmp = a[k * n + j];
            a[k * n + j] = a[index * n + j];
            a[index * n + j] = tmp;
        }
        tmp = b[k];
        b[k] = b[index];
        b[index] = tmp;

        
        for (int j = k + 1; j < n; ++j){
            a[k * n + j] /= max_el;
        }
        b[k] /= max_el;

        for (int i = k + 1; i < n; ++i){
            tmp = a[i * n + k];
            for (int j = k + 1; j < n; ++j){
                a[i * n + j] -= a[k * n + j] * tmp;

            }
            b[i] -= b[k] * tmp;
        }
    }
    for (int i = n - 1; i >= 0; --i){
        tmp = 0;
        for (int j = i + 1; j < n; ++j){
            tmp += a[i * n + j] * x[j];
        }
        x[i] = b[i] - tmp;    
    }
    return 0;
}

