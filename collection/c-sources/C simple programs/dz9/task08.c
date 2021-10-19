#include <time.h>

#include "func.h"

#define TASK_NUM 8

int main(int argc, char *argv[]){
	double res1 = 0, res2 = 0;
    double x = 0, e = 0;
    double t = 0;

	if(!(argc == 3 && sscanf(argv[1], "%lf", &x) == 1 && sscanf(argv[2], "%lf", &e) == 1)){
		printf("Usage: %s x e\n", argv[0]);
		return 1;
	}

	t = clock();
	res1 = task08(x, e);
	t = (clock() - t) / CLOCKS_PER_SEC; 
	res2 = fabs(log(x) - res1);

	printf ("%s : Task = %d Result = %e Residual = %e Elapsed = %.2f\n", argv[0], TASK_NUM, res1, res2, t);

	return 0;
}