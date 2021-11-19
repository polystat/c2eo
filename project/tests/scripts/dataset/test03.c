struct st {
    int a;
    struct st2{ int x, y;} b;
} a = {1,{2,3}};
int b=17;
int main() {
    a.b.x;
    b;
    return 0;
}