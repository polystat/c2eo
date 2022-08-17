int
test()
{
	char a[16], b[16];
	
	if(sizeof(a) != sizeof(b))
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
