// Вычиление числа фибоначчи
// Использование глобальных, статических и локальных переменных
// а также передачи параметров и возврат результата
#include <stdio.h>

typedef long long i64;

i64 fibo(i64 n) {
  i64 p1 = 0;
  i64 p2 = 1;
  i64 result = 1;
  i64 i = 1;
  while (i < n) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
  return result;
}

int main() {
  i64 n = 10;
  i64 result = fibo(n);
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

  [param-start param-size] > fibo
    add param-start param-size > local-start
    address global-ram (add param-start 0) > n
    address global-ram (add local-start 0) > p1
    address global-ram (add local-start 8) > p2
    address global-ram (add local-start 16) > result
    address global-ram (add local-start 24) > i

    seq > @
      write p1 0
      write p2 1
      write result 1
      write i 1
      while
        less (read-as-int64 i) (read-as-int64 n)
        seq
          write result (add-int64 p1 p2)
          write p1 (read-as-int64 p2)
          write p2 (read-as-int64 result)
          write i (add (read-as-int64 i) 1)
      write return (read-as-int64 result)
      write return-size 8
      TRUE

  [param-start param-size] > main
    add param-start param-size > local-start
    address global-ram (add local-start 0) > n
    address global-ram (add local-start 8) > result
    seq > @
      write n 10
      write (address global-ram (add empty-global-position 0)) (read-as-int64 n)
      write empty-global-position (add empty-global-position 40)
      fibo (sub empty-global-position 40) 8
      write empty-global-position (add empty-global-position 40)
      write result (read-as-int64 return)
      printf "fibo(%d) = %d\n" (read-as-int64 n) (read-as-int64 result)
      TRUE

  [arg] > eo-application
    seq > @
      write empty-global-position (add empty-global-position 16)
      main (sub empty-global-position 16) 0
      write empty-global-position (sub empty-global-position 16)
      TRUE

  seq > @
    write empty-global-position 0
    eo-application args
    TRUE
*/
