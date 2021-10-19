#include "func.h"

const double eps = 10e-17;

int read_arrays(double* x, double* y, int n, const char* name){
    FILE *fp;
    int i, j;

    if (!(fp = fopen(name, "r"))){
      return ERROR_OPEN;
    }

    for (int i = 0; i < n; i ++)
    {
        if (fscanf(fp, "%lf", x + i) != 1)
        {
            fclose(fp);
            return ERROR_READ;
        }

        if (fscanf(fp, "%lf", y + i) != 1)
        {
            fclose(fp);
            return ERROR_READ;
        }
    }
    for(i = 0; i < n; ++i){
        for(j = i + 1; j < n; ++j){
            if(fabs(x[i] - x[j]) < eps){
                return ERROR_EQUAL;
            }
        }
    }
    fclose(fp);
    return ERROR_SUCCESS;
}


double task01(double x0, int n, double* x, double* y){
    int i, j;
    double tmp;
    double p = 1;
    double denom = 1;
    double polynom = 0; 

    for(i = 0; i < n; ++i){
        tmp = x[i];
        if (fabs(x0 - tmp) < eps){
            return y[i];
        }
        p *= x0 - tmp;
    }

    for (i = 0; i < n; i ++)
    {
        for (j = 0; j < n; ++j){
            if (i != j){
                denom *= x[i] - x[j];
            }
        }
        y[i] /= denom;
        denom = 1;
    }
    
    for(i = 0; i < n; ++i){
        polynom += y[i] / (x0 - x[i]);
    }

    return polynom * p;
}

double task02(double x0, int n, double* x, double* y){
    int i, j;
    double polynom = 0;

    for(i = 0; i < n; ++i){
        if (fabs(x0 - x[i]) < eps){
            return y[i];
        }
    }

    for (i = 0; i < n; ++i){
        for (j = n - 1; j > i; --j){
            y[j] = (y[j] - y[j - 1]) / (x[j] - x[j - i - 1]);
        }
    }
    for (i = n - 1; i >= 0; --i){
        polynom = y[i] + (x0 - x[i]) * polynom;
    }

    return polynom;
}

double task03(double x0, int n, double* x, double* y){
    int i, j;

    for(i = 0; i < n; ++i){
        if (fabs(x0 - x[i]) < eps){
            return y[i];
        }
    }

    for (i = 0; i < n; ++i){
	    for (j = 0; j < n - i - 1; ++j){
		    y[j] = (y[j + 1] * (x0 - x[j]) - y[j] * (x0 - x[i + j + 1])) / (x[j + i + 1] - x[j]);
        }
    }

	return y[0];
}





double tsin(double x, double e){
    double rem = x, tmp = x * x;
    double res = 0;
    double denom = 1;
    for(int i = 1; fabs(rem) >= e; i += 2)
    {
        res += rem;
        denom = (i + 1) * (i + 2);
        rem *= (-1) * tmp / denom;
    }

    return res;
}

double tcos(double x, double e){
    double rem = 1;
    double tmp = x * x; 
    double denom = 1;
    double res = 0; 

    for(int i = 0; fabs(rem) >= e; i+= 2)
    {
        res += rem;
        denom = (i + 1) * (i + 2);
        rem *= (-1) * tmp / denom;
    }

    return res;
}

double task05(double x, double e){
    int sgn = 1;
    //printf("sin = %lf\n", sin(x));
    if(x < 0){
        sgn = -1;
        x = -x;
    }
    x = fmod(x, 2 * M_PI);
    if(x >= M_PI){
        sgn *= -1;
        x -= M_PI;
    }
    if (x >= 0.75 * M_PI){
        sgn *= -1;
        return sgn * tsin(x - M_PI, e);
    }
    if(x >= 0.25 * M_PI){
        return sgn * tcos(x - 0.5 * M_PI, e);
    }
    
    return sgn * tsin(x, e);
}

double task06(double x, double e){
    int sgn = 1;
    //printf("cos = %lf\n", cos(x));
    if(x < 0){
        x = -x;
    }
    x = fmod(x, 2 * M_PI);
    if(x >= M_PI){
        sgn *= -1;
        x -= M_PI;
    }
    if (x >= 0.75 * M_PI){
        sgn *= -1;
        return sgn * tcos(x - M_PI, e);
    }
    if (x >= 0.25 * M_PI){
        sgn *= -1;
        return sgn * tsin(x - 0.5 * M_PI, e);
    }

    return sgn * tcos(x, e);
}

double task07(double x, double e){
    int i;
    int sgn = 0;
    double intgr_x, frct_x;
    double intgr_exp = 1, frct_exp = 0;
    double rem = 1;
    //printf("exp = %e\n", exp(x));
    if(x > 0){
        sgn = 1;
        x = -x;
    }
    intgr_x = ceil(x);
    frct_x = x - intgr_x;
    
    for(i = 0; i < fabs(intgr_x); ++i){
        intgr_exp *= M_E;
    }
    for (i = 1; fabs(rem) > e; ++i){
        frct_exp += rem;
        rem *= frct_x / i;
    }
    
    if (sgn > 0){
        return (1 / frct_exp) * intgr_exp;
    }

    return (1 / intgr_exp) * frct_exp;
}

double task08(double x, double e){
    double p = 0;
    double sum = 0;
    double tmp = 0;
    double res = 0;
    //printf("log = %e\n", log(x));
    while (x > 1){
        x /= 2;
        ++p;
    }
    res += p / M_LOG2E;
    tmp = 1 - x;
    for (int i = 1; fabs(tmp / i) >= e; ++i){
        sum += tmp / i;
        tmp *= (1 - x);
    }
    res -= sum;

    return res;
}


