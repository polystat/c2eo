int
test()
{
	int i;

	for(i = 0; i < 10; i++)
		if (!i)
			continue;
	
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
