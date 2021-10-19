#include <stdio.h>
#include <stdlib.h>

enum{
	READ_SUCCESS,
	READ_ERR_OPEN,
	READ_ERR_READ
};

double f(int, int, int, int, int);

void init_matrix(double*, int, int, int);
int read_matrix(double*, int, int, const char*);
void print_matrix(double*, int, int, int);

int task01(double*, int);
double task02(double*, int);
void task03(double*, int);
void task04(double*, int);
void task05(double*, int);
void task06(double*, int, int, int);
void task07(double*, int, int, int, int);
void task08(double*, int, int, int, double);
void task09(double*, double*, double*, int, int);
void task10(double*, double*, double*, int, int, int);