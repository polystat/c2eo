#include <stdio.h>
#include <stdlib.h>

struct Book {
    int price;
    char* title;
}

int main(int argc, char** argv) {
    struct Book b = {10, "title"};
    printf("Book.price = %d \n", b.price);
    printf("Book.title = %s \n", b.title);
}