enum{
	READ_SUCCESS,
	READ_ERR_OPEN,
	READ_ERR_READ
};

double f(int, int, int, int, int);

void init_matrix(double*, int, int, int);
int read_matrix(double*, int, int, const char*);
void print_matrix(double*, int, int, int);

int equal_zero(double x);

void mult1(double* a, double* x, double* x1, int n);
double scalar(double* x, double* x1, int n);
double r2_1(double* a, double* x, int n, double r1);
double f_r1(double* a, double* b, double* x, int n);
double f_r2(double* x, int n);
void mult2(double* a, double* x0, double* x, int n);
void make_b(double* a, double* b, int n);

double task01(double* a, double* x0, double* x, int n, int m);
void task02(double* a, double *b, double* x, double* x0, int n, int m, double t);
void task03(double* a, double* b, double* x, double* x0, double* r, int m, int n);
void task04(double* a, double* b, double* x, double* x0, double* r, int m, int n);
void task05(double* a, double* b, double* x, double* x0, double* r, int m, int n);
void task06(double* a, double* b, double* x, double* x0, double* r, int m, int n);
void task07(double* a, double* b, double* x, double* x0, double* r, int m, int n, double t);