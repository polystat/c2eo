#include <time.h>
#include <stdio.h>
#include <string.h>
#include "sort.h"
#include "stringio.h"

#define TASK_NUM 2

int main(int argc, char *argv[])
{
    char** a;
    char** b;
    char** c;
    int (*cmp) (char *, char *);
    int n = 0, m = 0, pa = 0, pb = 0, f = 0; 
    const char *namea;
    const char *nameb;
    int read_res = 0;
    int res = 0;
    double t = 0;

    if (argc != 8 || sscanf(argv[1], "%d", &f) != 1 || sscanf(argv[2], "%d", &n) != 1 
    || (n < 0) || sscanf(argv[3], "%d", &pa) != 1 
    || sscanf(argv[5], "%d", &m) != 1 || (m < 0) 
    || sscanf(argv[6], "%d", &pb) != 1)
    {
        printf("Usage : %s, c, n, pa, namea, m, pb, nameb\n", argv[0]);
        return 1;
    }
    namea = argv[4];
    nameb = argv[7];
    switch (f)
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
            printf("Incorret value of c = %d\n", f);
        return 2;
  	}

    a = (char**)malloc(n * sizeof(char*));
    if (!a)
    {
        printf("Can not allocate %d elements\n", n);
        return 3;
    }

    b = (char **)malloc(m * sizeof(char *));
    if(!b)
    {
        printf("Cannot allocate %d elements\n", m);
        free(a);
        return 3;
    }

    c = (char **)malloc((n + m) * sizeof(char *));
    if(!c)
    {
        printf("Cannot allocate %d elements\n", n + m);
        free(b);
        free(a);
        return 3;
    }
    read_res = read_array(a, n, namea);
    if (read_res != ERROR_SUCCESS)
    {
        switch(read_res)
        {
            case ERROR_OPEN:
                printf("Cannot open %s\n", namea);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", namea);
                break;
            case ERROR_MEM:
                printf("Memory error in %s\n", namea);
                break;
            default:
                printf("Unknown error in %s\n", namea);
        }
        free(b);
        free(c);
        return 4;
    }
    read_res = read_array(b, m, nameb);
    if (read_res != ERROR_SUCCESS)
    {
        switch(read_res)
        {
            case ERROR_OPEN:
                printf("Cannot open %s\n", nameb);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", nameb);
                break;
            case ERROR_MEM:
                printf("Memory error in %s\n", nameb);
                break;
            default:
                printf("Unknown error in %s\n", nameb);
        }
        free_array(a, n);
        free(c);
        return 5;
    }
    printf("Array a:\n");
    print_array(a, n, pa);
    printf("_______________\n");
    printf("Array b:\n");
    print_array(b, m, pb);
    printf("_______________\n");
    t = clock();
    sort02(a, b, c, n, m, cmp);
    t = (clock() - t) / CLOCKS_PER_SEC;
    /*printf("Sorted array:\n");
    print_array(a, n, p);*/
    printf("Array c:\n");
    print_array(c, m + n, pa + pb);
    free_array(a, n);
    free_array(b, n);
    free(c);
    printf("%s: Task = %d Result = %d Elapsed = %.2f\n", argv[0], TASK_NUM, res, t);
    return 0;
}
