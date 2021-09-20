// Тестирование описанния переменной, инициализированной константным выражением
int intVar01 = 3.3 + (5 * 2) - 10.2;
int intVar02;
int intVar03 = 0;
int intVar04 = 5 + 3 - 8;
extern int intVar05;
//extern int intVar06 = 10;
static int intVar07;
static int intVar08 = 8;

void func01 () {
    int locVar01;
    int locVar02 = 5;
    static int statLocVar01;
    static int statLocVar02 = 2;
}

void func02 (int par01) {}

int func03 () {return 5;}

int func04 (int par02) {return par02;}
