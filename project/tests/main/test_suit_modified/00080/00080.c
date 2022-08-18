void
voidfn()
{
    return;
}

int
test()
{
    voidfn();
    return 0;
}

#include <stdio.h>
int main()
{
    int x = test();
    printf("%d\n", x);
    return x;
}
