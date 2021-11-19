struct st {
    struct st2{ int x, y;} b;
} a;
int b=17;
int main() {
    a.b.x;
    b;
    return 0;
}