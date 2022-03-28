// Вычиление числа фибоначчи
// Использование глобальных переменных

#include <stdio.h>

typedef long long i64;

i64 p1 = 0;
i64 p2 = 1;
i64 n;
i64 result;
i64 i;

void fibo() {
  i = 1;
  while (i < n) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
}

int main() {
  n = 10;
  fibo();
  printf("fibo(%lld) = %lld\n", n, result);
  return 0;
}
/*
[args...] > global

  ram 2048 > global-ram
  memory > empty-global-position
  ram 1024 > return-ram
  memory > return-size
  address return-ram 0 > return
  address global-ram 0 > p1
  address global-ram 8 > p2
  address global-ram 16 > n
  address global-ram 24 > result
  address global-ram 32 > i

  [param-start param-size] > fibo
    add param-start param-size > local-start
    seq > @
      write i 1
      while
        less (read-as-int64 i) (read-as-int64 n)
        seq
          write result (add-int64 p1 p2)
          write p1 (read-as-int64 p2)
          write p2 (read-as-int64 result)
          write i (add (read-as-int64 i) 1)
      TRUE

  [] > main
    seq > @
      write n 10
      fibo 
      printf "fibo(%d) = %d\n" (read-as-int64 n) (read-as-int64 result)
      TRUE

  [n] > eo-application
    seq > @
      main
      TRUE

  seq > @
    write p1 0
    write p2 1
    write empty-global-position 40
    eo-application args
    TRUE
*/