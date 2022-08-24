struct T;

struct T {
	int x;
};

int
test()
{
	struct T v;
	{ struct T { int z; }; }
	v.x = 2;
	if(v.x != 2)
		return 1;
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
