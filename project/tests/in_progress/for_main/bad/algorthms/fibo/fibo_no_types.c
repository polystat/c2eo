// Вычисление числа фибоначчи
// Моделирование только глобальной памяти

#include <stdio.h>

typedef unsigned char byte;
typedef long long i64;

// Модель внешней памяти
byte ram[sizeof(i64)*6];
byte* p1 = ram;                      // &ram[0]
byte* p2 = ram + sizeof(i64);        // ram + 8
byte* arg  = ram + sizeof(i64)*2;    // ram + 16
byte* result = ram + sizeof(i64)*3;  // ram + 24
byte* i = ram + sizeof(i64)*4;       // &ram[32]

void init() {
  *(i64*)p1 = 0;
  *(i64*)p2 = 1;
  *(i64*)arg = 0;
  *(i64*)result = 0;
  *(i64*)i = 0;
}

void fibo() {
  *(i64*)i = 1;
  while (*(i64*)i < *(i64*)arg) {
    *(i64*)result = *(i64*)p1 + *(i64*)p2;
    *(i64*)p1 = *(i64*)p2;
    *(i64*)p2 = *(i64*)result;
    (*(i64*)i)++;
  }
}

int main() {
  init();     // Здесь, так как нельзя инициализировать до main
  *(i64*)arg = 10;
  fibo();
  printf("%lld\n", *(i64*)arg);
  i64 a;
  a = *(i64*)arg;
  a;
  printf("%lld\n", *(i64*)result);
  i64 r;
  r = *(i64*)result;
  r;
  return 0;
}
