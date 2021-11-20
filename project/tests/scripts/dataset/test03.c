struct st {
    int a;
    struct st2 {
        int x, y;
    } b;
} a = {1, {2, 3}},
        b = {1, 2, -3};

int main() {
    a.b.x;
    a.b.y;
    a.a;
    b.a;
    return 0;
}