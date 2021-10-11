#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);

    if (a == 5)
    {
      printf("IF work\n");
    }

    if (a == 5)
    {
      printf("IF-else work\n");
    }
    else
    {
      printf("if-ELSE work\n");
    }

    if (a == 5)
    {
      printf("IF-else_if work\n");
    }
    else if(a == 6)
    {
      printf("if-ELSE_IF work\n");
    }

    if (a == 5)
    {
      printf("IF-else_if-else work\n");
    }
    else if (a == 6)
    {
      printf("if-ELSE_IF-else work\n");
    }
    else
    {
      printf("if-else_if_ELSE work\n");
    }
}