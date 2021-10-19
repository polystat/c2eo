#include <math.h>
#include <stdio.h>

#define max_it8 30
#define max_it10 30

int getCount(void);
double sgn(double x);

double f0(double x);
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
double f5(double x);
double f6(double x);

double task01(double (*f)(double), double x, double h);
double task02(double (*f)(double), double x, double h);
double task03(double (*f)(double), double x, double h);
double task04(double (*f)(double), double a, double b, int n);
double task05(double (*f)(double), double a, double b, int n);
double task06(double (*f)(double), double a, double b, int n);
double task07(double (*f)(double), double a, double b, int n);
int task08(double (*f)(double), double a, double b, double e, double *x);
int task09(double (*f)(double), double a, double b, double e, double *x);
double task10(double (*f)(double), double a, double e, double *x);
double task11(double (*f)(double), double a, double e, double *x);