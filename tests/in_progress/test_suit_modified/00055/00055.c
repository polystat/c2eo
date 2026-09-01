enum E {
	x,
	y = 2,
	z,
};

int
test()
{
	enum E e;

	if(x != 0)
		return 1;
	if(y != 2)
		return 2;
	if(z != 3)
		return 3;
	
	e = x;
	return e;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
