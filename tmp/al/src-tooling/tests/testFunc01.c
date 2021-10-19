void do_math(int *x) {
    *x += 5;
}
 
void do_str(char *s) {
    *s = '5';
    s[0] = 'b';
}
 
int main(int argc, char **argv) {
    char *arg0 = argv[0];
    return 0;
}
