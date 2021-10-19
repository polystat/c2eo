#include <time.h>

#include "func.h"

#define TASK_NUM 6

int main(int argc, char *argv[]){
    node *head;
    int p = 0;
    char *name = 0;
    int read_res = 0;
    int res = 0;
    double t = 0;

    if (!(argc == 3 && sscanf(argv[1], "%d", &p) == 1 && p > 0)){
        printf("Usage: %s p file_name\n", argv[0]);
        return 1;
    }
    name = argv[2];
    read_res = read_list(name, &head);
    if(read_res != ERROR_SUCCESS){
            switch(read_res){
            case ERROR_OPEN: 
                printf("Cannot open %s\n", name);
                break;
            case ERROR_MALLOC: 
                printf("Cannot allocate memory for elements from %s\n", name);
                break;
            case ERROR_READ: 
                printf("Cannot read %s\n", name);
                break;
            default:
                printf("Unknown error in %s\n", name);
        }
        return 2;
    }
    print_list(p, head);

    t = clock();
    res = task06(head);
    t = (clock() - t) / CLOCKS_PER_SEC;
    delete_list(head);

    printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], TASK_NUM, res, t);
    return 0;
}

