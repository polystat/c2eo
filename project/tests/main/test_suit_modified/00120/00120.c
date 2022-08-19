struct {
	enum { X } x;
} s;


int
test()
{
	return X;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
