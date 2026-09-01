int
test()
{
	char *p;
	
	p = "hello";
	return p[0] - 104;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
