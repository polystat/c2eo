#include <stdio.h>

int
test(void)
{
 	printf("hello world\n");
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
