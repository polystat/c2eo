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

[] > c_bool

  memory > @

  sprintf "%b" $ > as-string

  "bool" > type
```

## 3. c_char

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[] > c_char

  memory > @

  "char" > type
```

## 4. c_float64

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[] > c_float64

  memory > @

  sprintf "%f" $ > as-string

  "float64" > type

  8 > bytes
```

## 5. c_int16

```java
+package c2eo.ctypes

+alias org.eolang.txt.sprintf

[] > c_int16

  memory > @

  [value] > write
    -32768 > min
    32767 > max
    65536 > range

    if. > @
      value.greater max
      ^.@.write (((value.add min).mod range).add min)
      if.
        value.less min
        ^.@.write (((value.add 32769).mod range).add max)
        ^.@.write value

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

[] > c_int32

  memory > @

  [value] > write
    -2147483648 > min
    2147483647 > max
    4294967296 > range

    if. > @
      value.greater max
      ^.@.write (((value.add min).mod range).add min)
      if.
        value.less min
        ^.@.write (((value.add 2147483649).mod range).add max)
        ^.@.write value

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

[] > c_int64

  memory > @

  sprintf "%d" $ > as-string

  "int64" > type

  8 > bytes

  -9223372036854775808 > min

  9223372036854775807 > max
```

## 8. Operators

### Assignment operator =

```c
float a = 0;
float b = 1;
b = a;
```

```java
cfloat > a
cfloat > b
b.write a
```

### Addition operator  +

```c
float a = 0;
float b = 1;
b = a + b;
```

```java
cfloat > a
cfloat > b
b.write (a.add b)
```

### Substraction operator -

```c
float a = 0;
float b = 1;
b = a - b;
```

```java
cfloat > a
cfloat > b
b.write (a.sub b)
```

### Multiply operator *

```c
float a = 0;
float b = 1;
b = a * b;
```

```java
cfloat > a
cfloat > b
b.write (a.mul b)
```

### Divide operator /

```c
float a = 0;
float b = 1;
b = a / b;
```

```java
cfloat > a
cfloat > b
b.write (a.div b)
```

### Mod operator %

```c
float a = 0;
float b = 1;
b = a % b;
```

```java
cfloat > a
cfloat > b
b.write (a.mod b)
```

### etc.

## 9. Сonstants

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

## 10. ComplexReturn

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
            error "Unexpected behavior"
      result
      error "Unexpected behavior"

  [] > main
    c_int32 > a
    c_int32 > result

    seq > @
      a.write (^.args.get 0).as-int
      result.write (^.f a)
      stdout (sprintf "complexReturn[%d] = %d\n" a result)
```

## 11. dowhile

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

## 12. enum

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

[args] > enumC1

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

[args] > enumC2

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

## 13. factorial

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

## 14. fibonacci

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

## 15. for

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

## 16. functionpointers

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

## 17. if

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

## 18. pi

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

## 19. simplereturn

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
      error "Unexpected behavior"

  [] > main
    c_int32 > a
    c_int32 > result

    seq > @
      a.write (^.args.get 0).as-int
      result.write (^.f a a)
      stdout (sprintf "simpleReturn[%d] = %d \n" a result)
```

## 20. struct

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

## 21. switch

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

## 22. typecasting

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

## 23. while

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

