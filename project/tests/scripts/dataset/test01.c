struct q {
    int a, b, c, d;
} a = {1, 2, 3, 4},
b = {2, 3, 4, 5};

int main() {
    a.b;
    b.b;
    a = b;
    a.b;
    return 0;
}