#include <stdio.h>

typedef long long i64;

i64 fibo(i64 p1, i64 p2, i64 n) {
  i64 tmp;
  if (0 < n) {
    tmp = p1;
    p1 = p1 + p2;
    p2 = tmp;
    n--;
    fibo(p1, p2, n);    
  } else {
    return p1;
  }
}

int main() {
  printf("fibo(%lld) = %lld\n", 10ll, fibo(0, 1, 10));
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
    address global-ram (add param-start 0) > p1
    address global-ram (add param-start 8) > p2
    address global-ram (add param-start 16) > n
    address global-ram (add local-start 0) > tmp

    seq > @
      if
        less 0 (read-as-int64 n)
        seq
          write tmp (read-as-int64 p1)
          write p1 (add-int64 p1 p2)
          write p2 (read-as-int64 tmp)
          write n (sub (read-as-int64 n) 1)
          write (address global-ram (add empty-global-position 0)) (read-as-int64 p1)
          write (address global-ram (add empty-global-position 8)) (read-as-int64 p2)
          write (address global-ram (add empty-global-position 16)) (read-as-int64 n)
          write empty-global-position (add empty-global-position 32)
          fibo (sub empty-global-position 32) 24
          write empty-global-position (add empty-global-position 32)
          TRUE
        seq
          write return (read-as-int64 p1)
          write return-size 8
          TRUE
      TRUE

  [param-start param-size] > main
    add param-start param-size > local-start
    seq > @
      write (address global-ram (add empty-global-position 0)) 0
      write (address global-ram (add empty-global-position 8)) 1
      write (address global-ram (add empty-global-position 16)) 10
      write empty-global-position (add empty-global-position 32)
      fibo (sub empty-global-position 32) 24
      write empty-global-position (add empty-global-position 32)
      printf "fibo(%d) = %d\n" 10 (read-as-int64 return)
      TRUE

  [arg] > eo-application
    seq > @
      write empty-global-position (add empty-global-position 0)
      main (sub empty-global-position 0) 0
      write empty-global-position (sub empty-global-position 0)
      TRUE

  seq > @
    write empty-global-position 0
    eo-application args
    TRUE
*/
