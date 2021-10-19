#include "func.h"

static int count = 0;

int getCount(void){
    return count;
}

double f0(double x){
    (void)x;
    ++count;
    return 1;
}

double f1(double x){
    ++count;
    return 1 + x;
}

double f2(double x){
    ++count;
    return 1 + x + x * x;
}

double f3(double x){
    ++count;
    return 1 + x + x * x + x * x * x;
}

double f4(double x){
    ++count;
    return 1 + x + x * x + x * x * x + x * x * x * x;
}

double f5(double x){
    ++count;
    return exp(-x);
}

double f6(double x){
    ++count;
    return 1 / (25 * x * x + 1);
}

double task01(double (*f)(double), double x, double h){
    return (f(x + h) - f(x)) / h;
}

double task02(double (*f)(double), double x, double h){
    return 0.5 * (f(x + h) - f(x - h)) / h;
}

///////////////////////////////////////////////////////////////////////////////
double task03(double (*f)(double), double x, double h){
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}
/////////////////////////////////////////////////////////////////////////////

double task04(double (*f)(double), double a, double b, int n){
    double h = (b - a) / n;
    double sum = 0;
    double xi;
    int i;
    sum += 0.5 * h * f(a);
    for(i = 1; i < n; ++i){
        xi = a + h * i;
        sum += h * f(xi);
    }
    sum += 0.5 * h * f(b);
    return sum;
}

double task05(double (*f)(double), double a, double b, int n){
    double h = (b - a) / n * 0.5;
    double sum = 0, sum1 = 0, sum2 = 0;
    double xi;
    int i;
    sum += h * f(a) / 3;
    for(i = 1; i < n; ++i){
        xi = a + h * 2 * i;
        sum1 += h * f(xi);
    }
    for(i = 0; i < n; ++i){
        xi = a + h * (2 * i + 1);
        sum2 += h * f(xi);
    }
    sum += h * f(b) / 3.0 + 2.0 / 3.0 * sum1 + 4 / 3.0 * sum2;
    return sum;
}

double sgn(double x){
    if(x < 0){
        return -1;
    }
    return 1;
}

double task06(double (*f)(double), double a, double b, int n){
    double h = (b - a) / n;
    double xi = a, fxi = f(xi);
    double xi1, fxi1;
    double ai = 0, bi = 0;
    double sum = 0;
    int i;
    for(i = 0; i < n; ++i){
        xi1 = xi + h;
        fxi1 = f(xi1);
        ai = fxi1 - fxi;
        bi = fxi - xi * ai;
        sum += ai * 2.0 / 3.0 * (fabs(xi1) * sqrt(fabs(xi1)) - fabs(xi) * sqrt(fabs(xi))) + bi * 2 * (sqrt(fabs(xi1)) * sgn(xi1) - sqrt(fabs(xi)) * sgn(xi));
        xi = xi1;
        fxi = fxi1;
    }
    return sum;
}

double task07(double (*f)(double), double a, double b, int n){
    double h = (b - a) / n * 0.5;
    double x1 = a, fx1 = f(x1);
    double x2, fx2;
    double x3, fx3;
    double ai = 0, bi = 0, gi = 0;
    double sum = 0;
    int i;
    for(i = 0; i < n; ++i){
        x2 = x1 + h;
        fx2 = f(x2);
        x3 = x2 + h;
        fx3 = f(x3);
        ai = (fx1 - 2 * fx2 + fx3) / (2 * h * h);
        bi = (fx2 - fx1) / h - (x1 + x2) * ai;
        gi = fx1 - x1 * x2 * ai - x1 * (fx2 - fx1) / h;
        sum += ai * 0.4 * (fabs(x3 * x3) * sqrt(fabs(x3)) * sgn(x3) - fabs(x1 * x1) * sqrt(fabs(x1)) * sgn(x1)) +
        bi * 2.0 / 3.0 * (fabs(x3) * sqrt(fabs(x3)) - fabs(x1) * sqrt(fabs(x1))) + gi * 2 * (sqrt(fabs(x3)) * sgn(x3) - sqrt(fabs(x1)) * sgn(x1));
        x1 = x3;
        fx1 = fx3;
    }
    return sum;
}

int task08(double (*f)(double), double a, double b, double e, double *res){
    int i, j;
    int n = 2;
    double h = (b - a) * 0.5;
    double tmp;
    double fa = f(a), fb = f(b);
    double xi;
    double sn = ((fa + fb) * 0.5 + f(a + h)) * h;
    double s2n;
    for(i = 1; i < max_it8; ++i){
        tmp = 0;
        h *= 0.5;
        xi = a + h;
        for(j = 0; j < n; ++j){
            tmp += f(xi) * h;
            xi += 2 * h;
        }
        s2n = 0.5 * sn + tmp;
        if (fabs(s2n - sn) < e) {
            *res = s2n;
            return 2 * n;
        }
        n *= 2;
        sn = s2n;
    }
    return -1;
}

double task10(double (*f)(double), double a, double e, double *res){
    int i;
    double b;
    int count = 0;
    int h = 1; 
    double tmp = 0;
    double integ = 0;
    for (i = 0; i < max_it10; ++i){
        b = a + h;
        count = task08((*f), a, b, e, &tmp);
        if (count < 0){
          return -1;
        }
        integ += tmp;
        if (fabs(tmp) < e){
            *res = integ;
            return b;
        }
        h = h * 2;
        a = b;
    }
    return -1;
}

int task09(double (*f)(double), double a, double b, double e, double *res){
    int i, j;
    int n = 2;
	double h = (b - a) * 0.5;
    double xi;
    double fa = f(a), fb = f(b);
	double sn_1 = h * (fa + fb) / 3.0;
	double sn_2 = h * 2 * f(a + h) / 3.0;
    double sn = sn_1 + 2 * sn_2;
    double s2n_1, s2n_2, s2n;
    double tmp;
	for(i = 1; i < max_it8; ++i){
	    s2n_1 = (sn_1 + sn_2) * 0.5;
		tmp = 0;
		h *= 0.5;
		xi = a + h;
        for(j = 0; j < n; ++j){
			tmp += f(xi) * h;
			xi += 2 * h;
		}
        s2n_2 = 2.0 * tmp / 3.0;
        s2n = s2n_1 + 2 * s2n_2;
		if(fabs(s2n - sn) < e){
			*res = s2n;
			return 2 * n;
		}
        n *= 2;
        sn_1 = s2n_1;
		sn_2 = s2n_2;
        sn = s2n;
	}
	return -1;
}

double task11(double (*f)(double), double a, double e, double *res){
    int i;
    double b;
    int count = 0;
    int h = 1; 
    double tmp = 0;
    double integ = 0;
    for (i = 0; i < max_it10; ++i){
        b = a + h;
        count = task08((*f), a, b, e, &tmp);
        if (count < 0){
          return -1;
        }
        integ += tmp;
        if (fabs(tmp) < e){
            *res = integ;
            return b;
        }
        h = h * 2;
        a = b;
    }
    return -1;
}






