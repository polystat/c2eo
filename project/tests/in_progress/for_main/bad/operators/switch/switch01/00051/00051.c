int x = 0;

int
test()
{
	switch(x)
		case 0:
			;
	switch(x)
		case 0:
			switch(x) {
				case 0:
					goto next;
				default:
					return 1;
			}
	return 1;
	next:
	switch(x)
		case 1:
			return 1;
	switch(x) {
		{
			x = 1 + 1;
			foo:
			case 1:
				return 1;
		}
	}
	switch(x) {
		case 0:
			return x;
		case 1:
			return 1;
		default:
			return 1;
	}
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
