struct S {int a; int b;};
struct S s = { .b = 2, .a = 1};

int
test()
{
	if(s.a != 1)
		return 1;
	if(s.b != 2)
		return 2;
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
