## 1. c_book

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[] > c_book

  c_int32 > price

  memory > title

  "struct" > type
```

## 2. c_bool

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_bool

  memory > mem

  if. > @
    mem.is-empty
    if.
      write initial
      mem
      seq
    mem

  [value] > write
    if. > @
      value.eq 0
      ^.mem.write FALSE
      ^.mem.write TRUE

  sprintf "%d" as-int > as-string

  $.if 1 0 > as-int

  "bool" > type
```

## 3. c_char

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_char

  memory > mem

  if. > @
    mem.is-empty
    if.
      mem.write initial
      mem
      seq
    mem

  mem.write > write

  "char" > type
```

## 4. c_float64

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_float64

  memory > mem

  if. > @
    mem.is-empty
    if.
      mem.write initial
      mem
      seq
    mem

  mem.write > write

  sprintf "%e" $ > as-string

  "float64" > type

  8 > bytes
```

## 5. c_int16

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_int16

  memory > mem

  if. > @
    mem.is-empty
    if.
      write initial
      mem
      seq
    mem


  [value] > write
    -32768 > min
    32767 > max
    65536 > range

    if. > @
      value.greater max
      ^.mem.write (((value.add min).mod range).add min)
      if.
        value.less min
        ^.mem.write (((value.add 32769).mod range).add max)
        ^.mem.write value

  sprintf "%d" $ > as-string

  "int16" > type

  4 > bytes

  32767 > max

  -32768 > min
```

## 6. c_int32

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_int32

  memory > mem

  if. > @
    mem.is-empty
    if.
      write initial
      mem
      seq
    mem

  [value] > write
    -2147483648 > min
    2147483647 > max
    4294967296 > range

    if. > @
      value.greater max
      ^.mem.write (((value.add min).mod range).add min)
      if.
        value.less min
        ^.mem.write (((value.add 2147483649).mod range).add max)
        ^.mem.write value

  sprintf "%d" $ > as-string

  "int32" > type

  4 > bytes

  -2147483648 > min

  2147483647 > max
```

## 7. c_int64

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_int64

  memory > mem

  if. > @
    mem.is-empty
    if.
      mem.write initial
      mem
      seq
    mem

  mem.write > write

  sprintf "%d" $ > as-string

  "int64" > type

  8 > bytes

  -9223372036854775808 > min

  9223372036854775807 > max
```

## 8. c_uint16

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_uint16

  memory > mem

  if. > @
    mem.is-empty
    if.
      write initial
      mem
      seq
    mem

  [value] > write
    0 > min
    65535 > max
    65536 > range

    if. > @
      value.greater max
      ^.mem.write (value.mod range)
      if.
        value.less min
        ^.mem.write (range.add (value.mod range))
        ^.mem.write value

  sprintf "%d" $ > as-string

  "uint16" > type

  2 > bytes

  0 > min

  65535 > max
```

## 9. c_uint32

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[initial] > c_uint32

  memory > mem

  if. > @
    mem.is-empty
    if.
      write initial
      mem
      seq
    mem

  [value] > write
    0 > min
    4294967295 > max
    4294967296 > range

    if. > @
      value.greater max
      ^.mem.write (value.mod range)
      if.
        value.less min
        ^.mem.write (range.add (value.mod range))
        ^.mem.write value

  sprintf "%d" $ > as-string

  "uint32" > type

  4 > bytes

  0 > min

  4294967295 > max
```

## 10. Сonstants

### Int

```c
const int constantName = 10;
```

```java
10 > constantName
```

### Float

```c
const float constantName = 3.14;
```

```java
3.14 > constantName
```

### Boolean

```c
const _Bool constantName = 1;
```

```java
TRUE > constantName
```

### Char

```c
const char constantName = 'a';
```

```java
"a" > constantName
```

## 11. globalc

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_bool
+alias c2eo.ctypes.c_char
+alias c2eo.ctypes.c_float64
+alias c2eo.ctypes.c_int16
+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_int64

[] > globalC

  seq > @
    non_initialized
    stdout "\n"
    initialized

  c_bool > b_1
  c_char > c_1
  c_float64 > f64_1
  c_int16 > i16_1
  c_int32 > i32_1
  c_int64 > i64_1

  seq > non_initialized

    b_1.write FALSE
    c_1.write 'a'
    f64_1.write 3.1415
    i16_1.write 127
    i32_1.write 255
    i64_1.write 511

    stdout (sprintf "b_1 = %b\n" b_1)
    stdout (sprintf "c_1 = %c\n" c_1)
    stdout (sprintf "f64_1 = %f\n" f64_1)
    stdout (sprintf "i16_1 = %d\n" i16_1)
    stdout (sprintf "i32_1 = %d\n" i32_1)
    stdout (sprintf "i64_1 = %d\n" i64_1)


  c_bool TRUE > b_2
  c_char 'b' > c_2
  c_float64 3.1415 > f64_2
  c_int16 128 > i16_2
  c_int32 256 > i32_2
  c_int64 512 > i64_2

  seq > initialized
    stdout (sprintf "b_2 = %b\n" b_2)
    stdout (sprintf "c_2 = %c\n" c_2)
    stdout (sprintf "f64_2 = %f\n" f64_2)
    stdout (sprintf "i16_2 = %d\n" i16_2)
    stdout (sprintf "i32_2 = %d\n" i32_2)
    stdout (sprintf "i64_2 = %d\n" i64_2)
```

## 12. operationsc

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[] > operationsC

  c_int32 11 > a
  c_int32 5 > b
  c_int32 > c

  seq > @

    stdout (sprintf "a = %d\n" a)
    stdout (sprintf "b = %d\n" b)

    c.write (a.add b)
    stdout (sprintf "c = a + b  ->  c = %d + %d  ->  c = %d\n" a b c)
    c.write (a.sub b)
    stdout (sprintf "c = a - b  ->  c = %d - %d  ->  c = %d\n" a b c)
    c.write (a.mul b)
    stdout (sprintf "c = a * b  ->  c = %d * %d  ->  c = %d\n" a b c)
    c.write (a.div b)
    stdout (sprintf "c = a / b  ->  c = %d / %d  ->  c = %d\n" a b c)
    c.write (a.mod b)
    stdout (sprintf "c = a mod b  ->  c = %d mod %d  ->  c = %d\n" a b c)
```

## 13. overflowc

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int16
+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_int64

[] > overflowC

  seq > @
    non_initialized
    stdout "\n"
    initialized

  c_int16 > i16_1
  c_int32 > i32_1
  c_int64 > i64_1

  seq > non_initialized

    i16_1.write 65000
    i32_1.write 9223372036324
    i64_1.write 9223372036854775807

    stdout (sprintf "int16 = %d  ->  int16 = %d\n" 65000 i16_1)
    stdout (sprintf "int32 = %d  ->  int32 = %d\n" 9223372036324 i32_1)
    stdout (sprintf "int64 = %d  ->  int64 = %d\n" 9223372036854775807 i64_1)

    i16_1.write -65000
    i32_1.write -9223372036324
    i64_1.write -9223372036854775808

    stdout (sprintf "int16 = %d  ->  int16 = %d\n" -65000 i16_1)
    stdout (sprintf "int32 = %d  ->  int32 = %d\n" -9223372036324 i32_1)
    stdout (sprintf "int64 = %d  ->  int64 = %d\n" -9223372036854775808 i64_1)


  c_int16 65000 > i16_2
  c_int32 9223372036324 > i32_2
  c_int64 9223372036854775807 > i64_2

  c_int16 -65000 > i16_3
  c_int32 -9223372036324 > i32_3
  c_int64 -9223372036854775808 > i64_3

  seq > initialized

    stdout (sprintf "int16 %d  ->  int16 = %d\n" 65000 i16_2)
    stdout (sprintf "int32 %d  ->  int32 = %d\n" 9223372036324 i32_2)
    stdout (sprintf "int64 %d  ->  int64 = %d\n" 9223372036854775807 i64_2)

    stdout (sprintf "int16 %d  ->  int16 = %d\n" -65000 i16_3)
    stdout (sprintf "int32 %d  ->  int32 = %d\n" -9223372036324 i32_3)
    stdout (sprintf "int64 %d  ->  int64 = %d\n" -9223372036854775808 i64_3)
```

## 14. break

- C

```c
#include <stdio.h>
#include <stdlib.h>

int breakOrReturn(int a)
{
    while (1)
    {
        if ((a % 5) == 0)
        {
            break;
        }

        if ((a % 3) == 0)
        {
            return 1;
        }

        a++;
    }

    printf("We broke out of the loop\n");
    return 0;
}


int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = breakOrReturn(a);
    printf("breakOrReturn = %d", result);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_bool

[args] > breakC

  [a] > breakOrReturn

    c_int32 > a1
    c_int32 > result
    c_bool > isBreak
    c_bool > isReturn

    if. > @
      seq
        a1.write a
        isBreak.write FALSE
        isReturn.write FALSE
        while.
          TRUE.and (isBreak.not) (isReturn.not)
          [i]
            seq > @
              if.
                (^.a1.mod 5).eq 0
                seq
                  ^.isBreak.write TRUE
                seq
              if.
                (^.a1.mod 3).eq 0
                seq
                  ^.isReturn.write TRUE
                  ^.result.write 1
                seq
              if.
                ^.isReturn
                seq
                  ^.a1.write (^.a1.add 1)
                seq
        if.
          isReturn
          seq
          if.
            seq
              stdout "We broke out of the loop\n"
            result.write 0
            seq

      result
      seq

  [] > main

    c_int32 > a
    c_int32 > result

    seq > @
      a.write (^.args.get 0).as-int
      result.write (^.breakOrReturn a)
      stdout (sprintf "breakOrReturn = %d" result)
```

## 15. complexreturn

- C

```c
#include <stdio.h>
#include <stdlib.h>

int f(int a) {

  if (a == 1)
  {
    a = a + 1;
  }

  if (a == 2)
  {
      a = a + 2;
      return 2;
  }
  else if (a == 3)
  {
      a = a + 3;
      return 3;
  }

  if (a == 4)
  {
    a = a + 4;
  }

  a = a + 5;
  return a;
}

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = f(a);
    printf("complexReturn[%d] = %d", a, result);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_bool

[args] > complexReturnC

  [a] > f
    c_int32 > result
    c_bool > isReturn

    if. > @
      seq
        isReturn.write FALSE
        if.
          a.eq 1
          seq
            a.write (a.add 1)
          seq

        if.
          a.eq 2
          seq
            a.write (a.add 2)
            isReturn.write TRUE
            result.write 2
          if.
            a.eq 3
            seq
              a.write (a.add 3)
              isReturn.write TRUE
              result.write 3
            seq

        if.
          isReturn
          seq
          if.
            seq
              if.
                a.eq 4
                seq
                  a.write (a.add 4)
                seq
            seq
              a.write (a.add 5)
              isReturn.write TRUE
              result.write a
            seq
      result
      seq

  [] > main
    c_int32 > a
    c_int32 > result

    seq > @
      a.write (^.args.get 0).as-int
      result.write (^.f a)
      stdout (sprintf "complexReturn[%d] = %d\n" a result)
```

## 16. continue

- C

```c
#include <stdio.h>
#include <stdlib.h>

int collatzProblem(int number)
{
    while (number > 1)
    {
        if ((number % 2) == 0 )
        {
            number = number / 2;
            continue;
        }

        number = (number * 3) + 1;
    }

    return number;
}


int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = collatzProblem(a);
    printf("collatzProblem(%d) = %d", a, result);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_bool
+alias c2eo.ctypes.c_int32

[args] > continueC

  [number] > collatzProblem

    c_int32 number > number1
    c_bool FALSE > isContinue

    if. > @
      seq
        while.
          number1.greater 1
          [x]
            seq > @
              if.
                (^.number1.mod 2).eq 0
                seq
                  ^.number1.write (^.number1.mod 2)
                  ^.isContinue.write TRUE
                seq
              if.
                ^.isContinue
                seq
                ^.number1.write ((^.number1.mul 3).add 1)
      number1
      seq

  [] > main

    c_int32 > a
    c_int32 > result

    seq > @
      a.write (^.args.get 0).as-int
      result.write (^.collatzProblem a)
      stdout (sprintf "collatzProblem(%d) = %d" a result)
```

## 17. dowhile

- C

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int i = 0;
    do
    {
        printf("do while[%d] ", i);
        i++;
    } while (i < a);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > dowhileC

  c_int32 > a
  c_int32 > i

  [] > cycle_body
    seq > @
      stdout (sprintf "dowhile[%d] " (^.i))
      ^.i.write (^.i.add 1)

  seq > main
    a.write (args.get 0).as-int
    i.write 0

    cycle_body
    while.
      i.less a
      [x]
        ^.cycle_body > @
```

## 18. enum

- C

```c
#include <stdio.h>
#include <stdlib.h>


enum week { Mon, Tue, Wed, Thur, Fri, Sat, Sun};

enum week currentDay = Sun;

enum week getSomeDay() {
    return currentDay;
}

int main(int argc, char** argv) {
  enum week someday, anotherDay;
  someday = Wed;
  printf("enum someday = %d\n", someday);
  anotherDay = getSomeDay();
  printf("enum another day = %d\n", anotherDay);
  return 0;
}
```

- EO ver. 1

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

0 > mon
1 > tue
2 > wed
3 > thur
4 > fri
5 > sat
6 > sun

[args] > enum1C

  sun > currentDay
  c_int32 > someday
  c_int32 > anotherDay

  currentDay > getSomeDay

  seq > main
    someday.write 2
    stdout (sprintf "enum1 someday = %d\n" someday)
    anotherDay.write getSomeDay
    stdout (sprintf "enum1 another day = %d\n" anotherDay)
```

- EO ver. 2

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > enum2C

  6 > currentDay
  c_int32 > someday
  c_int32 > anotherDay

  currentDay > getSomeDay

  seq > main
    someday.write 2
    stdout (sprintf "enum2 someday = %d\n" someday)
    anotherDay.write getSomeDay
    stdout (sprintf "enum2 another day = %d\n" anotherDay)
```

## 19. factorial

- C

```c
#include <stdio.h>
#include <stdlib.h>

int n;
int result = 1;

void factorial() {
    if(n < 2) {
        return;
    } else {
        result = result * n;
        n = n - 1;
        factorial();
    }
}

int main(int argc, char** argv) {
    n = atoi(argv[1]);
    printf("%d! = ", n);
    factorial();
    printf("%d\n", result);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.txt.sprintf
+alias org.eolang.io.stdout

+alias c2eo.ctypes.c_int32

[args] > factorialC

  c_int32 > n
  c_int32 > result

  if. > factorial
    n.less 2
    seq
    seq
      result.write (n.mul result)
      n.write (n.sub 1)
      factorial

  seq > main
    result.write 1
    n.write (args.get 0).as-int
    stdout (sprintf "%d! = " n)
    factorial
    stdout (sprintf "%d\n" result)
```

## 20. fibonacci

- C

```c
#include <stdio.h>
#include <stdlib.h>

int n;
int result = 1;
int lastResult = 0;
int tmp;

void fibonacci() {
    if(n < 1) {
        return;
    } else {
        tmp = result;
        result = result + lastResult;
        lastResult = tmp;
        n = n - 1;
        fibonacci();
    }
}

int main(int argc, char** argv) {
    n = atoi(argv[1]);
    printf("fibonacci[%d] = ", n);
    fibonacci();
    printf("%d\n", lastResult);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.txt.sprintf
+alias org.eolang.io.stdout

+alias c2eo.ctypes.c_int32

[args] > fibonacciC

  c_int32 > n
  c_int32 > result
  c_int32 > lastResult
  c_int32 > tmp

  if. > fibonacci
    n.less 1
    seq
    seq
      tmp.write result
      result.write (result.add lastResult)
      lastResult.write tmp
      n.write (n.sub 1)
      fibonacci

  seq > main
    result.write 1
    lastResult.write 0
    n.write (args.get 0).as-int
    stdout (sprintf "fibonacci[%d] = " n)
    fibonacci
    stdout (sprintf "%d\n" result)
```

## 21. for

- C

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    for(int i = 0; i < a; i++) {
      printf("for[%d] ", i);
   }
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > forC

  c_int32 > a
  c_int32 > i

  seq > main
    a.write (args.get 0).as-int
    i.write 0
    while.
      i.less a
      [x]
        seq > @
          stdout (sprintf "for[%d] " (^.i))
          ^.i.write (^.i.add 1)
```

## 22. functionpointers

- C

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct { int price; } Book;

typedef int (*read) (Book* b);

int f(Book* b){
    printf("price is %d", b->price);
}

void g(read r, Book* b) {
    r(b);
}

int main(int argc, char** argv) {
    Book b = { 7 };
    Book* pb = &b;
    g(f, pb);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_book

[b] > f
  seq > @
    stdout (sprintf "price is %d" b.price)

[r b] > g
  seq > @
    r b

[args] > functionPointersC

  c_book > b

  seq > main
    b.price.write 7
    g f b
```

## 23. if

- C

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);

    if (a == 5)
    {
      printf("IF work\n");
    }

    if (a == 5)
    {
      printf("IF-else work\n");
    }
    else
    {
      printf("if-ELSE work\n");
    }

    if (a == 5)
    {
      printf("IF-else_if work\n");
    }
    else if(a == 6)
    {
      printf("if-ELSE_IF work\n");
    }

    if (a == 5)
    {
      printf("IF-else_if-else work\n");
    }
    else if (a == 6)
    {
      printf("if-ELSE_IF-else work\n");
    }
    else
    {
      printf("if-else_if_ELSE work\n");
    }
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > ifC

  c_int32 > a

  seq > main
    a.write (args.get 0).as-int

    if.
      a.eq 5
      seq
        stdout "IF work\n"
      seq

    if.
      a.eq 5
      seq
        stdout "IF-else work\n"
      seq
        stdout "if-ELSE work\n"

    if.
      a.eq 5
      seq
        stdout "IF-else_if work\n"
      if.
        a.eq 6
        seq
          stdout "if-ELSE_IF work\n"
        seq

    if.
      a.eq 5
      seq
        stdout "IF-else_if-else work\n"
      if.
        a.eq 6
        seq
          stdout "if-ELSE_IF-else work\n"
        seq
          stdout "if-else_if_ELSE work\n"
```

## 24. pi

- C

```c
#include <stdio.h>
#include <stdlib.h>

int n;
int i = 0;
float divider = 1.0;
float result = 0;

void pi() {
    if(i < n) {
        result = result + (1.0 / ((i % 2 == 0) ? divider : -divider));
        divider = divider + 2.0;
        i = i + 1;
        pi();
    } else {
        result = result * 4.0;
        return;
    }
}

int main(int argc, char** argv) {
    n = atoi(argv[1]);
    printf("pi[%d] = ", n);
    pi();
    printf("%f\n", result);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.txt.sprintf
+alias org.eolang.io.stdout

+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_float64

[args] > piC

  c_int32 > n
  c_int32 > i
  c_float64 > divider
  c_float64 > result

  if. > pi
    i.less n
    seq
      result.write
        result.add
          1.0.div
            if.
              (i.mod 2).eq 0
              divider
              divider.neg

      divider.write (divider.add 2.0)
      i.write (i.add 1)
      pi

    seq
      result.write (result.mul 4.0)

  seq > main
    n.write (args.get 0).as-int
    i.write 0
    divider.write 1.0
    result.write 0.0
    stdout (sprintf "pi[%d] = " n)
    pi
    stdout (sprintf "%f\n" result)
```

## 25. pointers1

- C

```c
// Работа с указателями
// Формирование значений через указатели

#include <stdio.h>
int x = 5;
int *p;

int main() {
    int y;
    p = &x;
    *p = 25;
    p = &y;
    *p = x;
    printf("x = %d\ny = %d\n*p = %d\n", x, y, *p);
    return 0;
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.txt.sprintf
+alias org.eolang.io.stdout

+alias c2eo.ctypes.c_int32

[args] > pointers1C

  c_int32 5 > x

  [] > main

    c_int32 > y

    seq > @
      ^.x.write 25
      y.write (^.x)
      stdout (sprintf "x = %d\ny = %d\n*p = %d\n" (^.x) y y)
```

## 26. pointers2

- C

```c
// Работа с указателями
// Формирование значений через void-указатели

#include <stdio.h>
int x = 5;
void *p;

int main() {
    int y;
    p = &x;
    *(int*)p = 25;
    p = &y;
    *(int*)p = x;
    printf("x = %d\ny = %d\n*p = %d\n", x, y, *(int*)p);
    return 0;
}

```

- EO

```java
+package c2eo.examples

+alias org.eolang.txt.sprintf
+alias org.eolang.io.stdout

+alias c2eo.ctypes.c_int32

[args] > pointers2C

  c_int32 5 > x

  [] > main

    c_int32 > y

    seq > @
      ^.x.write 25
      y.write (^.x)
      stdout (sprintf "x = %d\ny = %d\n*p = %d\n" (^.x) y y)
```

## 27. pointers3

- C

```c
// Работа с указателями
// Формирование значений через void-указатели

#include <stdio.h>
int x = 5;
int *p;
int **pp;

int main() {
    int y;
    p = &x;
    *p = 25;
    pp = &p;
    p = &y;
    y = x + 10;
    int ***ppp = &pp;
    printf("x = %d\ny = %d\n*p = %d\n**pp = %d\n***ppp = %d\n",
           x, y, *p, **pp, ***ppp);
    return 0;
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.txt.sprintf
+alias org.eolang.io.stdout

+alias c2eo.ctypes.c_int32

[args] > pointers3C

  c_int32 5 > x

  [] > main

    c_int32 > y

    seq > @
      ^.x.write 25
      y.write (^.x.add 10)
      stdout (sprintf "x = %d\ny = %d\n*p = %d\n**pp = %d\n***ppp = %d\n" (^.x) y y y y)
```

## 28. simplereturn

- C

```c
#include <stdio.h>
#include <stdlib.h>

int f(int a, int b) {
  int c = a + b;
  return c;
}

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int result = f(a, a);
    printf("simpleReturn[%d] = %d \n", a, result);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > simpleReturnC

  [a b] > f

    c_int32 > c

    if. > @
      seq
        c.write (a.add b)
      c
      seq

  [] > main
    c_int32 > a
    c_int32 > result

    seq > @
      a.write (^.args.get 0).as-int
      result.write (^.f a a)
      stdout (sprintf "simpleReturn[%d] = %d \n" a result)
```

## 29. struct

- C

```c
#include <stdio.h>
#include <stdlib.h>

struct Book {
    int price;
    char* title;
}

int main(int argc, char** argv) {
    struct Book b = {10, "title"};
    printf("Book.price = %d \n", b.price);
    printf("Book.title = %s \n", b.title);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_book

[args] > structC

  c_book > b

  seq > main
    b.price.write 10
    b.title.write "title"
    stdout (sprintf "Book.price = %d \n" (b.price))
    stdout (sprintf "Book.title = %s \n" (b.title))
```

## 30. switch

- C

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    a = atoi(argv[1]);
    printf("switch[%d] = ", a);
    switch (a) {
     case 0:
       printf("0");
       break;
     case 1:
       printf("1");
       break;
     case 5:
       printf("5");
       break;
     default:
       printf("default");
       break;
   }
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > switchC

  c_int32 > a

  seq > main
    a.write (args.get 0).as-int
    stdout (sprintf "switch[%d] = " a)
    if.
      a.eq 0
      seq
        stdout "0"
      if.
        a.eq 1
        seq
          stdout "1"
        if.
          a.eq 5
          seq
            stdout "5"
          seq
            stdout "default"
```

## 31. typecasting

- C

```c
#include <stdio.h>
#include <stdlib.h>

struct Book { int price; char* title; };

void f(void* b) {
    char* title = ((struct Book*) b)->title;
    printf("The title: %s", title);
}

int main() {
    struct Book b = {10, "some"};
    f(&b);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_book

[b] > f

  memory > title

  seq > @
    title.write (b.title)
    stdout (sprintf "The title: %s" title)

[args] > typeCastingC

  c_book > b

  seq > main
    c_book.price.write 10
    c_book.title.write "some"
    f c_book

```

## 32. while

- C

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int i = 0;
    while (i < a)
    {
        printf("while[%d] ", i);
        i++;
    }
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > whileC

  c_int32 > a
  c_int32 > i

  seq > main
    a.write (args.get 0).as-int
    i.write 0
    while.
      i.less a
      [x]
        seq > @
          stdout (sprintf "while[%d] " (^.i))
          ^.i.write (^.i.add 1)
```

## 33. nestedBlocks

- C

```c
#include <stdio.h>

static int a = 10;

int main() {
    int* pa = &a;
    int a = 20;
    printf("a = %d\n", a);
    {
        int a = 30;
        printf("a = %d\n", a);
        {
            int a = 40;
            printf("a = %d\n", a);
            {
                int a = 50;
                printf("a = %d\n", a);
            }
        }
    }

    printf("a = %d\n", *pa);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > nestedBlocksC

  [] > main

    c_int32 20 > a

    seq > @
      stdout (sprintf "a = %d\n" a)
      []
        c_int32 30 > a
        seq > @
          stdout (sprintf "a = %d\n" a)
          []
            c_int32 40 > a
            seq > @
              stdout (sprintf "a = %d\n" a)
              []
                c_int32 50 > a
                seq > @
                  stdout (sprintf "a = %d\n" a)
      stdout (sprintf "a = %d\n" a)
```

## 34. nestedBlocksStatic

- C

```c
#include <stdio.h>

static int a = 10;

int foo() {
    a += 1;
    printf("a = %d\n", a);
    static int* pa = &a;
    static int a = 20;
    a += 3;
    printf("a = %d\n", a);
    {
        static int a = 30;
        a += 7;
        printf("a = %d\n", a);
        {
            static int a = 40;
            a += 15;
            printf("a = %d\n", a);
            {
                static int a = 50;
                a += 31;
                printf("a = %d\n", a);
            }
        }
    }
    printf("a = %d\n", *pa);
}

int main() {
    foo();
    foo();
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > nestedBlocksStaticC

  c_int32 10 > a

  [] > foo
    c_int32 20 > a
    seq > @
      ^.a.write (^.a.add 1)
      stdout (sprintf "a = %d\n" (^.a))
      a.write (a.add 3)
      stdout (sprintf "a = %d\n" a)
      foo_1
      stdout (sprintf "a = %d\n" (^.a))

    [] > foo_1
      c_int32 30 > a
      seq > @
        a.write (a.add 7)
        stdout (sprintf "a = %d\n" a)
        foo_2

      [] > foo_2
        c_int32 40 > a
        seq > @
          a.write (a.add 15)
          stdout (sprintf "a = %d\n" a)
          foo_3

        [] > foo_3
          c_int32 50 > a
          seq > @
            a.write (a.add 31)
            stdout (sprintf "a = %d\n" a)

  [] > main
    seq > @
      ^.foo
      ^.foo
```

## 35. nestedBlocksMain

- C

```c
#include <stdio.h>

static int a = 10;

int main() {
    static int* pa = &a;
    static int a = 20;
    printf("a = %d\n", a);
    {
        static int a = 30;
        printf("a = %d\n", a);
        {
            static int a = 40;
            printf("a = %d\n", a);
            {
                static int a = 50;
                printf("a = %d\n", a);
            }
        }
    }
    printf("a = %d\n", *pa);
}
```

- EO

```java
+package c2eo.examples

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

+alias c2eo.ctypes.c_int32

[args] > nestedBlocksMainC

  c_int32 10 > a

  [] > main

    c_int32 20 > a
    c_int32 30 > a_1
    c_int32 40 > a_1_1
    c_int32 50 > a_1_1_1

    seq > @
      stdout (sprintf "a = %d\n" a)
      stdout (sprintf "a = %d\n" a_1)
      stdout (sprintf "a = %d\n" a_1_1)
      stdout (sprintf "a = %d\n" a_1_1_1)
      stdout (sprintf "a = %d\n" (^.a))

```
