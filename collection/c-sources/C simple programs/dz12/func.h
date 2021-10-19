#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN 1000

enum{ 
    ERROR_OPEN, 
    ERROR_MALLOC,  
    ERROR_READ, 
    ERROR_SUCCESS
};

struct _node;
typedef struct _node
{
    char *string;
    struct _node *next;
} node;

void delete_list(node *head);
int read_list(const char *name, node **head);
void print_list(int p, node *head);
int length_list(node *head);

int task01(node *head);
int task02(node *head);
int task03(node *head);
int task04(node *head);
int task05(node *head);
int task06(node *head);
int task07(node *head);
node* task08(node *head);
node* task09(node *head);