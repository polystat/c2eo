int g;

int
effect()
{
	g = 1;
	return 1;
}

int
test()
{
    int x;
    
    g = 0;
    x = 0;
    if(x && effect())
    	return 1;
    if(g)
    	return 2;
    x = 1;
    if(x && effect()) {
    	if(g != 1)
    		return 3;
    } else {
    	return 4;
    }
    g = 0;
    x = 1;
    if(x || effect()) {
    	if(g)
    		return 5;
    } else {
    	return 6;
    }
    x = 0;
    if(x || effect()) {
    	if(g != 1)
    		return 7;
    } else {
    	return 8;
    } 
    return 0;
}


#include <stdio.h>
int main()
{
    int x = test();
    printf("%d\n", x);
    return x;
}
