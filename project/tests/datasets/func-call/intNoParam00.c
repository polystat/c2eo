int a = 5;
int b = 10;
int c = 15;

int func_int1(){
    return a;
}

int func_int2(){
    return 10;
}

int main() {
    b = func_int1();
    c = func_int2();
    return 0;
}   

