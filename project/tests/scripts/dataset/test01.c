struct q{
    int a1:4;
    float b;
    char c:7;
} a;

struct {
    int a1:4;
    float b;
    char c:7;
} a1;

union qq{
    int a1:4;
    float b;
    char c:7;
} b;

union {
    int a1:4;
    float b;
    char c:7;
} b1;

struct w{
    int a:4, :6, :9, d:10;
};

struct e{
    struct r{
        int h;
    } g;
    union t{
        int k;
    }y;
};

int main(){
    return 0;
}