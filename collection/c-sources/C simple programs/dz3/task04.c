#include <time.h>
#include <stdio.h>
#include <string.h>
#include "sort.h"
#include "stringio.h"

#define TASK_NUM 4

int main(int argc, char *argv[])
{
    char ** a;
    int (*cmp) (char *, char *);
    int n = 0, p = 0, c = 0; 
    const char * name;
    int read_res = 0;
    double t;

    if (argc != 5 || (sscanf(argv[1], "%d", &c) != 1) || (sscanf(argv[2], "%d", &n) != 1)
     || (n < 0) || (sscanf(argv[3], "%d", &p) != 1))
    {
        printf("Usage : %s, x, c, n, p, name\n", argv[0]);
        return 1;
    }

    switch (c)
    {
        case 1: 
            cmp = cmp_lex_inc;
            break;
        case 2: 
            cmp = cmp_lex_dec;
            break;
        case 3: 
            cmp = cmp_len_inc;
            break;
        case 4: 
            cmp = cmp_len_dec;
            break;
        default:
            printf("Incorret value of c = %d\n", c);
        return 2;
  	}
    name = argv[4];

    a = (char**)malloc(n * sizeof(char*));
    if (!a)
    {
        printf("Can not allocate %d elements\n", n);
        return 3;
    }
    read_res = read_array(a, n, name);
    if (read_res != ERROR_SUCCESS)
    {
        switch(read_res)
        {
            case ERROR_OPEN:
                printf("Cannot open %s\n", name);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", name);
                break;
            case ERROR_MEM:
                printf("Memory error in %s\n", name);
                break;
            default:
                printf("Unknown error in %s\n", name);
        }
        return 4;
    }
    printf("Array:\n");
    print_array(a, n, p);
    printf("_______________\n");
    t = clock();
    sort04(a, n, cmp);
    t = (clock() - t) / CLOCKS_PER_SEC;
    printf("Sorted array:\n");
    print_array(a, n, p);
    free_array(a, n);
    printf("%s: Task = %d Elapsed = %.2f\n", argv[0], TASK_NUM, t);
    return 0;
}
