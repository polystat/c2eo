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

double abs_(double x);

void del_col(double*, int, int, int);
void del_crs(double*, int, int, int, int);

int task01(double*, int, int);
int task02(double*, int, int);
int task03(double*, int, int);
int task04(double*, int, int);
int task05(double*, int, int);
int task06(double*, int, int);
int task07(double*, int, int);
int task08(double*, int, int);

int task10(double*, int, int);

