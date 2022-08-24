int c;

int
test()
{
	if(0) {
		return 1;
	} else if(0) {
	} else {
		if(1) {
			if(c)
				return 1;
			else
				return 0;
		} else {
			return 1;
		}
	}
	return 1;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
