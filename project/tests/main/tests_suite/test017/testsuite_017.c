#include <stdio.h>
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
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}