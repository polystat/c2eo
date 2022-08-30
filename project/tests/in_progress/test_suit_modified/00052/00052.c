int
test()
{
	struct T { int x; };
	{
		struct T s;
		s.x = 0;
		return s.x;
	}
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
