int
test()
{
        int x;

        x = 3;
        x = !x;
        x = !x;
        x = ~x;
        x = -x;
        if(x != 2)
                return 1;
        return 0;
}

#include <stdio.h>
int main()
{
        int x = test();
        printf("%d\n", x);
        return x;
}
