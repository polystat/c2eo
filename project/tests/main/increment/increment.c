#include <stdio.h>

typedef long long i64;

i64 x = 42;
i64 y = 0;

i64 main() {
    x = 1 + x;
    printf("%ld\n", x);
    x;
    x = x + 1;
    printf("%ld\n", x);
    x;
    return 0;
}

/*
[args...] > global

  ram 2048 > global-ram
  memory > empty-global-position
  ram 1024 > return-ram
  memory > return-size
  address return-ram 0 > return
  address global-ram 0 > x

  [] > main
    seq > @
      write x (add (read-as-long long64 x) 1)
      printf "%d\n" (read-as-long long64 x)
      write x (add 1 (read-as-long long64 x))
      printf "%d\n" (read-as-long long64 x)
      TRUE

  [arg] > eo-application
    seq > @
      main
      TRUE

  seq > @
    write x 1
    write empty-global-position 8
    eo-application args
    TRUE
*/
