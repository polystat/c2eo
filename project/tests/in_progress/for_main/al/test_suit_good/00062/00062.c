#ifdef FOO
	XXX
#ifdef BAR
	XXX
#endif
	XXX
#endif

#define FOO 1

#ifdef FOO

#ifdef FOO
int x = 0;
#endif

int
test()
{
	return x;
}
#endif


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}



