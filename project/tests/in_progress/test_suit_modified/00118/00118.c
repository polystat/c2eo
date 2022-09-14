int
test()
{
	struct { int x; } s = { 0 };
	return s.x;
}

//#include <stdio.h>
int main()
{
	int x = test();
//	printf("%d\n", x);
	return x;
}
