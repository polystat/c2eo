#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum{
	READ_SUCCESS,
	READ_ERR_OPEN,
	READ_ERR_READ
};

double f(int, int, int, int, int);

void init_matrix(double*, int, int, int);
int read_matrix(double*, int, int, const char*);
void print_matrix(double*, int, int, int);

double f_r1(double* a, double* b, double* x, int n);
double f_r2(double* x, int n);
void make_b(double* a, double* b, int n);