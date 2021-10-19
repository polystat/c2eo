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
    return x - 1e100;
}

double f2(double x){
    ++count;
    return 4 - x * x;
}

double f3(double x){
    ++count;
    return x * x * x + 3 * x * x + 16;
}

double f4(double x){
    ++count;
    return 3 - 2 * x * x - 4 * x * x * x * x;
}

double f5(double x){
    ++count;
    return sqrt(fabs(x) + 1) - 2;
}

double f6(double x){
    ++count;
    return sqrt(sqrt(fabs(x) + 1) + 1) - 2;
}

double df0(double x){
	(void) x;
	return 0;
}

double df1(double x){
	(void) x;
	return 1;
}

double df2(double x){
	return (-2 * x);
}

double df3(double x){
	return (3 * x * x + 6 * x);
}

double df4(double x){
	return ((-4) * x - 4 * x * x * x);
}

double df5(double x){
	if(x > 0){
		return (0.5 / sqrt(x + 1));
	}
	if(x < 0){
        return (-0.5/ sqrt(1 - x));
    }
    return 0;
}

double df6(double x){
	if(x > 0){
		return (0.25/(sqrt(x + 1) * sqrt(sqrt(x + 1) + 1)));
	}
	if(x < 0){
        return (-0.25/(sqrt(1 - x) * sqrt(sqrt(1 - x) + 1)));
    }
    return 0;
}

int sgn(double x){
    if(x < 0){
        return -1;
    }
    if(x > 0){
        return 1;
    }
    return 0;
}

int task01(double (*f)(double), double a, double b, double e, double* x){
    double fa = f(a);
    double fb = f(b);
    double c = 0, fc = 0;
    int i;
    if(fabs(fa) < e){
		*x = a;
		return 0;
	}
	if(fabs(fb) < e){
		*x = b;
		return 0;
	}
    for(i = 0; i < max_it; ++i){
        c = (a + b) * 0.5;
        if ((c <= a && c >= a) || (c <= b && c >= b)){
            *x = c;
            return i;
        }
        fc = f(c);
        if (fabs(fc) < e){
			*x = c;
			return i; 
		}
        if(sgn(fa) * sgn(fc) <= 0){
            b = c;
            fb = fc;
        } else if(sgn(fb) * sgn(fc) <= 0){
            a = c;
            fa = fc;
        } else {
            break;
        }
    }
    return -1;
}

int task02(double (*f)(double), double (*df)(double), double x0, double e, double *x){
    double fx0 = 0, dfx0 = 0;
    int i;
    for(i = 0; i < max_it; ++i){
        fx0 = f(x0);
        if(fabs(fx0) < e){
            *x = x0;
            return i;
        }
        dfx0 = df(x0);
        if(fabs(dfx0) < e){
            return -1;
        }
        x0 -= fx0 / dfx0;
    }
    return -1;
}

int task03(double (*f)(double), double a, double b, double e, double* x){
    double fa = f(a);
    double fb = f(b);
    double c = 0, fc = 0;
    int i = 0;
    if(fabs(fa) < e){
		*x = a;
		return i;
	}
	if(fabs(fb) < e){
		*x = b;
		return i;
	}
    for(i = 0; i < max_it; ++i){
        if(fabs(fb - fa) > e){
            c = a - fa * (b - a) / (fb - fa);
        } else {
            return -1;
        }
        fc = f(c);
        if ((fabs(fc) < e) || (a <= c && a >= c) || (b <= c && b >= b)){
            *x = c;
            return i;
        }
        if(sgn(fa) * sgn(fc) <= 0){
            b = c;
            fb = fc;
        } else if(sgn(fb) * sgn(fc) <= 0){
            a = c;
            fa = fc;
        } else {
            break;
        }
    }
    return -1;
}

int task04(double (*f)(double), double a, double b, double e, double* x){
    double fa = f(a);
    double fb = f(b);
    double c = 0, fc = 0;
    int i = 0;
    if(fabs(fa) < e){
		*x = a;
		return i;
	}
	if(fabs(fb) < e){
		*x = b;
		return i;
	}
    for(i = 0; i < max_it; ++i){
        if(fabs(fb - fa) > e){
            c = a - fa * (b - a) / (fb - fa);
        } else {
            return -1;
        }
        fc = f(c);
        if ((fabs(fc) < e) || (a <= c && a >= c) || (b <= c && b >= b)){
            *x = c;
            return i;
        }
        if(fabs(fb) >= fabs(fc) && fabs(fb) >= fabs(fa)){
            b = a;
            a = c;
            fb = fa;
            fa = fc;
        } else if(fabs(fa) >= fabs(fc) && fabs(fa) >= fabs(fb)){
            a = b;
            b = c;
            fa = fb;
            fb = fc;
        } else {
            return -1;
        }
    }

    return -1;
}

int task07(double (*f)(double), double x0, double e, double *x){
    int i = 0;
	double fx0 = 0;
	for(i = 0; i < max_it; ++i){
		fx0 = f(x0);
		if(fabs(fx0 - x0) < e){
			*x = fx0;
			return i;
		}
		x0 = fx0;
	}
	return -1;
}

int task08(double (*f)(double), double a, double b, double e, double* x){
    int i = 0;
    double delta = 0.1;

    double step = (b - a) * delta;
    double f0 = f(a);
    double f1 = f(b);

    double fmax, xmax;
    double fi, xi = a;

    if(f0 > f1){
        fmax = f0;
        xmax = a;
    } else {
        fmax = f1;
        xmax = b;
    }

    if (fabs(step) < e){
		*x = xmax;
		return i;
	}

    for(i = 0; i < max_it; ++i){
        xi += step;
        fi = f(xi);
        if(fi < f0 || xi + fabs(step) > b || xi - fabs(step) < a){
            step *= -delta;
        }
        if(fabs(step) < e){
            *x = (fmax > fi ? xmax : xi);
            return i;
        }
    }
    return -1;
}


int task09(double (*f)(double), double a, double b, double e, double* x){
    int i;
    double fact = (sqrt(5) - 1) * 0.5;
    double x1 = a + fact * (b - a), fx1 = f(x1);
    double x2 = b - fact * (b - a), fx2 = f(x2);
    for(i = 0; i < max_it; ++i){
        if (fx1 > fx2) {
			b = x2;
			x2 = x1;
			fx2 = fx1;
			x1 = a + fact * (x2 - a);
			fx1 = f(x1);
		} else {
			a = x1;
			x1 = x2;
			fx1 = fx2;
			x2 = b - fact * (b - x1);
			fx2 = f(x2);
		}
        if(fabs(b - a) < e){
            *x = (fx1 > fx2 ? x1 : x2);
            return i;
        }
    }
    return -1;
}


