#include <math.h>

#define max_it 200000



int getCount(void);

double f0(double x);
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
double f5(double x);
double f6(double x);

double df0(double x);
double df1(double x);
double df2(double x);
double df3(double x);
double df4(double x);
double df5(double x);
double df6(double x);

int sgn(double x);

int task01(double (*f)(double), double a, double b, double e, double* x);
int task02(double (*f)(double), double (*df)(double), double x0, double e, double *x);
int task03(double (*f)(double), double a, double b, double e, double* x);
int task04(double (*f)(double), double a, double b, double e, double* x);

int task07(double (*f)(double), double x0, double e, double *x);
int task08(double (*f)(double), double a, double b, double e, double* x);
int task09(double (*f)(double), double a, double b, double e, double* x);