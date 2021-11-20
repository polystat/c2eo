struct st {
    int a;
    struct st2 {
        int x, y;
    } b;
} a = {1, {2, 3}},
b= {-2, {-7, 6*4*9}};

int main() {
    a.b.y;
    b.b.y;
    a.b = b.b;
    a.b.y;
    return 0;
}