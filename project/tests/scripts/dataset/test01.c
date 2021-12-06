int main() {
    struct q {
        int b;
        struct q2{
            struct q3{
                int h;
            } h;
        } a;
        struct q4{
            int h;
        }h;
    };
    struct q a = {1,2,3};
    return 0;
}