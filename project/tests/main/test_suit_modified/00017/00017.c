int
test()
{
	struct { int x; int y; } s;
	
	s.x = 3;
	s.y = 5;
	return s.y - s.x - 2; 
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
