int
zero()
{
	return 0;
}

struct S
{
	int (*zerofunc)();
} s = { &zero };

struct S *
anon()
{
	return &s;
}

typedef struct S * (*fty)();

fty
go()
{
	return &anon;
}

int
test()
{
	return go()()->zerofunc();
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
