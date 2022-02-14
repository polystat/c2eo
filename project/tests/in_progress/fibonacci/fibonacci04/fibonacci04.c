// Вычиление числа фибоначчи
// Использование глобальных, статических и локальных переменных
// а также передачи параметров и возврат результата

#include <stdio.h>

typedef long long i64;

i64 fibo(i64 arg) {
  i64 p1 = 0;
  i64 p2 = 1;
  i64 result = 1;
  i64 i = 1;
  while (i < arg) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
  return result;
}

int main() {
  i64 arg = 10;
  i64 result = fibo(arg);
  printf("fibo(%lld) = %lld\n", arg, result);
  return 0;
}
/*
+package c2eo.examples.fibonacci4

+alias org.eolang.gray.cage
+alias c2eo.system.ram
+alias c2eo.system.address
+alias c2eo.stdio.printf
+alias c2eo.string.memcpy
+alias c2eo.coperators.while
+alias c2eo.coperators.write
+alias c2eo.coperators.get
+alias c2eo.coperators.add
+alias c2eo.coperators.less
+alias c2eo.coperators.add-int64
+alias c2eo.coperators.as-int64
+alias c2eo.coperators.memadrcpy
+alias c2eo.coperators.adrmemcpy

[args...] > global

  * (* cage cage cage) > allocator
  memory > allocator-index

  ram 0 > global-ram
  ram 0 > static-ram
  ram 8 > arguments-ram
  ram 8 > result-ram

  [] > eo-init
    seq > @
      write (get (get allocator 0) 0) global-ram
      write (global-ram.index) 0
      write (get (get allocator 0) 1) static-ram
      write (static-ram.index) 1
      write (get (get allocator 0) 2) arguments-ram
      write (arguments-ram.index) 2
      write (get (get allocator 0) 3) result-ram
      write (result-ram.index) 3
      write allocator-index 4
      TRUE

  [] > fibo
    ram 8 > local-arguments-ram
    address local-arguments-ram 0 > p1 > arg
    ram 32 > local-ram
    address local-ram 0 > p1
    address local-ram 8 > p2
    address local-ram 16 > result
    address local-ram 24 > i
    seq > @
      memcpy local-arguments-ram arguments-ram 8
      write (get (get allocator 0) allocator-index) local-arguments-ram
      write allocator-index (add allocator-index 1)
      write (get (get allocator 0) allocator-index) local-ram
      write allocator-index (add allocator-index 1)
      write p1 0
      write p2 1
      write result 1
      write i 1
      while
        less (as-int64 i) (as-int64 arg)
        seq
          write result (add-int64 p1 p2)
          write p1 (as-int64 p2)
          write p2 (as-int64 result)
          write i (add 1 (as-int64 i))
          TRUE
      memadrcpy result-ram result
      write allocator-index (sub allocator-index 2)
      TRUE

  [args] > main
    ram 16 > local-ram
    address local-ram 0 > arg
    address local-ram 8 > result
    seq > @
      write (get (get allocator 0) allocator-index) local-ram
      write allocator-index (add allocator-index 1)
      write arg 10
      write arguments-ram 0 (as-bytes arg)
      fibo
      adrmemcpy result (result-ram) 8
      printf "fibo(%d) = %d\n" (as-int64 arg) (as-int64 result)
      TRUE

  seq > @
    eo-init
    main args
    TRUE
*/
