#include "functions.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[]) 
{
    char *f_in = 0, *f_out = 0, *str = 0, *tr = 0;
    int res;
    double t;
    if (argc != 5) {
        printf("Usage %s name_a name_b s t\n", argv[0]);
        return 1;
    }
    f_in = argv[1];
    f_out = argv[2];
    str = argv[3];
    tr = argv[4];
    
    t = clock();
    res = f4(f_in, f_out, str, tr);
    t = (clock() - t)/CLOCKS_PER_SEC;

    if (res < 0) {
        switch(res) {
            case -1:
                printf("Error: cannot open file\n");
                break;
            default:
                printf("Error: incorrect file %s content\n", f_in);
        }
        return 2;
    }
    printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], 4, res, t);
    return 0;
}
