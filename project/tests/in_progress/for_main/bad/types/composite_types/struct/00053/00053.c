int
test()
{
	struct T { int x; } s1;
	s1.x = 1;
	{
		struct T { int y; } s2;
		s2.y = 1;
		if (s1.x - s2.y != 0)
			return 1;
	}
	return 0;
}

//#include <stdio.h>
int main()
{
	int x = test();
//	printf("%d\n", x);
	return x;
}
