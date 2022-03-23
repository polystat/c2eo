#include <stdio.h>

typedef long long i64;

i64 fun1(i64 a, i64 b) {
  i64 result = a + b;
  result = fun2(result, 8);
  return result;
}

i64 fun2(i64 a, i64 b) {
  i64 result = a + b;
  return result;
}

i64 main() {
    printf("result = %d", fun1(7, 9));
    return 0;
}
/*
[args...] > global

  ram 2048 > global-ram
  memory > empty-global-position
  ram 1024 > return-ram
  memory > return-size
  address return-ram 0 > return

  [param-start param-size] > fun1
    add param-start param-size > local-start
    address global-ram (add param-start 0) > a
    address global-ram (add param-start 8) > b
    address global-ram (add local-start 0) > result
    seq > @
      write result (add-int64 a b)
      write (address global-ram (add empty-global-position 0)) (read-as-int64 result)
      write (address global-ram (add empty-global-position 8)) 8
      write empty-global-position (add empty-global-position 24)
      fun2 (sub empty-global-position 24) 16
      write empty-global-position (sub empty-global-position 24)
      write result (read-as-int64 return)
      write return-size 8
      write return (read-as-int64 result)
      TRUE

  [param-start param-size] > fun2
    add param-start param-size > local-start
    address global-ram (add param-start 0) > a
    address global-ram (add param-start 8) > b
    address global-ram (add local-start 0) > result
    seq > @
      write result (add-int64 a b)
      write return-size 8
      write return (read-as-int64 result)
      TRUE

  [] > main
    seq > @
      write (address global-ram (add empty-global-position 0)) 7
      write (address global-ram (add empty-global-position 8)) 9
      write empty-global-position (add empty-global-position 24)
      fun1 (sub empty-global-position 24) 16
      write empty-global-position (sub empty-global-position 24)
      printf "result = %d" (read-as-int64 return)
      TRUE

  [arg] > eo-application
    seq > @
      main
      TRUE

  seq > @
    write empty-global-position 0
    eo-application args
    TRUE
*/