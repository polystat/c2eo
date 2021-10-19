#include <stdio.h>
#include <stdlib.h>

typedef struct { int price; } Book;

typedef int (*read) (Book* b);

int f(Book* b){
    printf("price is %d", b->price);
}

void g(read r, Book* b) {
    r(b);
}

int main(int argc, char** argv) {
    Book b = { 7 };
    Book* pb = &b;
    g(f, pb);
}