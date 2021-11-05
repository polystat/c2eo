struct{
    char a;
    int b:5, c:11,:0, d:8;
    struct { int ee:8; } e;
    union {struct {
            int alltypes;
        } n;}q;
}a;
union {
    struct {
        int alltypes;
    } n;
    struct {
        int type;
        int intnode;
    } ni;
    struct {
        int type;
        double doublenode;
    } nf;
    struct {};
} u;
int main(){
    a;
    return 0;
}