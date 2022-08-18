typedef struct { int x; int y; } s;

s v;

int
test()
{
	v.x = 1;
	v.y = 2;
	return 3 - v.x - v.y;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
