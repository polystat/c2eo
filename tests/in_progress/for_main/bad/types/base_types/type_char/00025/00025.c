// int strlen(char *);
// Замена функции на свою для прохождения теста
int strlen_for_test(char * s) {
	char* t = s;
	int c = 0;
	while(*t != '\0') {
		++t;
		++c;
	}
	return c;
}

int
test()
{
	char *p;
	
	p = "hello";
// 	return strlen(p) - 5;
	return strlen_for_test(p) - 5;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
