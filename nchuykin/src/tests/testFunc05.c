float ff1 = 7;
static float ff2;
float ff3 = 17;

void do_math(int *x);

void do_str() {;;;;;;;;;;;}

void do_str0() {}

static int f1() {return 10;}

inline int f2() {return 20;}

int f3(int k) {return k;}

int inside(int argc, double x) {
    char t;
    int k = 5;
    ff2 = ff1;
    {
        k = k+1;
        x = 3.14;
    }
    return k;
}

int main(int argc, char **argv) {
    char *arg0 = argv[0];
    int k = 5;
    ff2 = ff1;
    k = k+1;
    return 0;
}
