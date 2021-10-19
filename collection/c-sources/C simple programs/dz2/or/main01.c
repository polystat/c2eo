#include <stdio.h>
#include <time.h>
#include "funcs.h"

int main (int argc, char* argv[])
{
	char *nameA = 0;
	char *nameB = 0;
	int res = 0;
	double t;
	int task = 1;
	char *s = 0;
	
	
	
	if (argc != 4)
	{
		printf("Usage: %s fin fout s\n", argv[0]);
		return 1;
	}
	
	
	nameA = argv[1];
	nameB = argv[2];
	s = argv[3];

	t = clock();
	res = task01(nameA, nameB, s);
	t = (clock() - t)/CLOCKS_PER_SEC;

	if (res < 0)
	{
		switch (res)
		{
			case -1:
				printf("Can't open %s\n", nameA);
				break;
			case -2:
				printf("Can't open %s\n", nameB);
				break;
			case -3:
				printf("Can't read %s %s\n", nameA, nameB);
				break;
			default:
				printf("Unknown error\n");
		}
		return 2;
	}

	printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, t);
	
	return 0;
}
