int main(int argc, char **argv);

int a = 3;
int b = 4;
int c;
 
int main(int argc, char **argv) {
    c = a + b;
    c = a;
    a = b;
    b = c;
}
