#define x f
#define y() f

typedef struct { int f; } S;

int
test()
{
	S s;

	s.x = 0;
	return s.y();
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
