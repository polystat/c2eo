// line comment

int
test()
{
	/*
		multiline
		comment
	*/
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
