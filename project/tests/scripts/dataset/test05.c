struct st {
    int a;
    struct st2 {
        int x, y;
    } b;
} a = {1, {2, 3}},
        b= {5, {6, 7}};

int main() {
    //a.b.y += 3;
    a.b.y = a.b.y + 3;

    //b.b.y -= 5;
    b.b.y = b.b.y - 5;

    //a.b.y *= b.b.x;
    a.b.y = a.b.y * b.b.x;
    return 0;
}
