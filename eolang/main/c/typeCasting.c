#include <stdio.h>
#include <stdlib.h>

struct Book { int price; char* title; };

void f(void* b) {
    char* title = ((struct Book*) b)->title;
    printf("The title: %s", title);
}

int main() {
    struct Book b = {10, "some"};
    f(&b);
}