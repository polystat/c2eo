int a = 5;
int b = 10;

void do_math() {
    a = b;
}

void do_print() {
    a;
}

int main() {
    do_print();
    do_math();
    do_print();
    return 0;
}
