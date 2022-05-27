<img src="https://www.yegor256.com/images/books/elegant-objects/cactus.svg" height="92px" /> 

![building](https://img.shields.io/github/workflow/status/polystat/c2eo/Build%20C2EO)
![version](https://img.shields.io/github/v/release/polystat/c2eo?include_prereleases)
![license](https://img.shields.io/github/license/polystat/c2eo?color=e6e6e6)
![Lines of code](https://tokei.rs/b1/github/polystat/c2eo)
![Hits-of-Code](https://hitsofcode.com/github/polystat/c2eo?branch=master)

This is a experimental translator of [C](https://en.wikipedia.org/wiki/C_(programming_language)) programs to [EO](https://www.eolang.org) programs.

## How to Use

Assuming, you are on [Ubuntu 20+](https://ubuntu.com/download):

```bash
$ sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F7C91591CC543ECA
$ sudo add-apt-repository 'deb http://c2eo.polystat.org/debian/ c2eo-rep non-free main contrib'
$ sudo apt install c2eo
```

Then, just run:

```bash
$ ./c2eo <path-to-c-file-name> <eo-file-name>.eo
# ./c2eo ../some_dir/example.c example.eo
```

 We do not support the utility for other distributions and operating systems yet. However, you can try to build the project from source at your own risk. 

## How to Contribute

### Preparations

Again, we recommend [Ubuntu 20+](https://ubuntu.com/download) and you will need
[wget](https://www.tecmint.com/install-wget-in-linux/), 
[tar](https://www.tecmint.com/install-tar-in-centos-rhel-and-fedora/), 
[git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git),
[cmake 3.18+](https://cmake.org/download/), 
[gcc](http://mirror.linux-ia64.org/gnu/gcc/releases/), 
[g++](https://pkgs.org/download/g++), 
[ninja-build](https://ninja-build.org/)
and
[python3.+](https://www.python.org/downloads/).
Also requirements from [the EO project](https://github.com/objectionary/eo) ([Maven](https://maven.apache.org) and Java 8+)

Then, you need to install [LLVM/Clang 12.0.1](https://github.com/llvm/llvm-project/releases/tag/llvmorg-12.0.1) :

```bash
$ wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-12.0.1.tar.gz
$ tar -xvf llvmorg-12.0.1.tar.gz
$ mv ./llvm-project-llvmorg-12.0.1 ./llvm-clang
$ cd llvm-clang
$ mkdir build && cd $_
$ cmake --no-warn-unused-cli -DBUILD_SHARED_LIBS:STRING=ON -DLLVM_TARGETS_TO_BUILD:STRING=X86 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE "-DLLVM_ENABLE_PROJECTS:STRING=clang;compiler-rt" -DCMAKE_BUILD_TYPE:STRING=Debug -DLLVM_OPTIMIZED_TABLEGEN:STRING=ON -DLLVM_USE_SPLIT_DWARF:STRING=ON -DLLVM_USE_LINKER:STRING=gold ../llvm -G Ninja
$ cmake --build . --config Debug --target all -j 10 -- -j1 -l 2
$ cd ../..
```

You may also try our own [pre-packaged archive](https://mega.nz/file/cZ9WQCqB#z713CuC-GNFQAXIxZwZxI05zOH4FAOpwYHEElgOZflA):

```bash
$ sudo apt install megatools
$ megadl 'https://mega.nz/#!cZ9WQCqB!z713CuC-GNFQAXIxZwZxI05zOH4FAOpwYHEElgOZflA'
$ tar -xvf llvm-clang.tar.gz
```

It is assumed that the `llvm-clang` is located in the `c2eo` dir. If your `llvm-clang` is in different place, set the path in that [line](https://github.com/polystat/c2eo/blob/3f687397f245658ee4ec14583b20fe114c873b15/project/src/transpiler/CMakeLists.txt#L7).

Formally speaking, this is where the preparation can be completed. However, in order to fully work with the project, testing and executing the translated code, you need to study the [EO compiler project](https://github.com/objectionary/eo) and fulfill its necessary requirements. After that, it will be possible to proceed with further steps.

### Making changes

All sources files of transpiler are located in `project/src/transpiler`. Аfter making changes in these files, we will need to rebuild the executable file `c2eo`. To do this, you need to go to the `project/build` dir and run the following commands:

```bash
$ cmake ..
$ make
``` 
As you have already noticed, the project is being built in the `project/build` folder. The result of this build is the `c2eo` file in `project/bin`. Now you have a transpiler and you can convert programs from C to EO. Just run:

```bash
$ ./c2eo <path-to-c-file-name> <eo-file-name>.eo
# ./c2eo ../some_dir/example.c example.eo
```
Ok, it works, but you're not going to manually broadcast each file and check if everything is OK. To do this, there are a couple of scripts that will simplify your life:

```bash
# Transpile and run all c files in folder, then compare their results and show statistics
$ python3 test.py ../tests/main

# Only transpile all c files, EO files are located in the /result dir
$ python3 transpile_c2eo.py ../tests/main

# Single-threaded launch of c2eo without formatting the output to the console for all c files
$ python3 c2eo-all ../tests/main

# Show code lines statistics in this dir 
$ python3 code_lines.py ../tests/main
```

The main tests are in the folder `/project/tests/main`, if they pass, then everything is ok. [Here](./project/scripts/readme.md) you can find more information about scripts.

## Principles of Transpilation from C to EO

C is a _system-level procedural_ programming language with direct access to the underlying hardware architecture elements, such as memory and registers. EO, on the other hand is a _high-level object-oriented_ language. There are a number of non-trivial mechanisms of translating constructs from the former to the latter, which are explained below:

:heavy_check_mark: [Implemented](#implemented):
- [basic data types: double, int, bool](#direct-memory-access-for-basic-data-types)
- [arrays](#arrays)
- [structures](#structures)
- [unions](#unions)
- [functions](#functions)
- [function call operators](#function-call-operators)
- [single return at the end of the function](#single-return)
- [pointers](#pointers)
- [external links](#external-links)
- [if-else](#if-else)
- [while-do](#while-do)
- [for](#for)
- [unary operations with base types, pointers and their and assignment modifications](#operators)
- [binary operations with base types and assignment modifications](#operators)

:hammer: In progress:
- [initial initialization of global variables](#global-initialization)
- [calling functions with local variables from other functions](#calling-functions-with-local-variables-from-other-functions)
- [bit operators (inconsistent implementation in the EO)](#bit-operators)
- [char, unsigned + short + int, float (not supported by EO)](#basic-types)

:x: [Not implemented](#not-implemented):
- [multiple return](#multiple-return)
- [switch case default](#switch-case-default)
- [const](#const)
- [enums](#enums)
- [break](#break)
- [continue](#continue)
- [goto and labels](#goto-and-labels)
- [calling functions with variable number of arguments](#calling-functions-with-variable-number-of-arguments)

### :heavy_check_mark: Implemented:

### Direct memory access for basic data types

Let's take the following C code as an example:

```c
double z = 3.14;
```

In EO, we represent the global memory space as a copy of [ram](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/system/ram.eo) object, which we call `global`. Thus, the variable `z` would be accessed as a block of 8 bytes inside `ram` at the very begginning, since it's the first variable seen. For example, to change the value of `z` we write 8 bytes to the 0th position of `global`:

```java
ram > global
global.write 0 (3.14.as-bytes)
```

### Arrays

If we have fixed-size arrays we can work like with one-dimension array and calculate bias from start for any element and dimensions. In this example, we use a special object [address](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/system/address.eo), which makes it more convenient to read and write information from memory from a certain position. 

```c
int a[2] = { 5, 6 };
╭─────┬─────╮
|  5  │  6  │
├─────┼─────┤
| 0th │ 4th │
╰─────┴─────╯
```

```java
address global-ram 0 > a
a.write (4.mul 0) (5.as-bytes)
a.write (4.mul 1) (6.as-bytes)
```

### Structures

We know the size of structures so we generate additional objects that store the bias of the fields of the structure and allow access to them. For nested structures and other types, we can also calculate bias and generate corresponding objects.

```c
struct Rectangle {int x; int y;} rect;
rect.x = 5;
╭───────┬───────╮
| int x │ int y │
├───────┼───────┤
|  0th  │  4th  │
╰───────┴───────╯
```

```java
address global-ram 0 > rect
0 > x
4 > y
(rect.add x).write 5
```

### Unions

The size of the union is determined by the nested object with the maximum size. The main feature is that internal objects are located at the beginning of the same address. We do the same with nested structures.

```c
union { int a; int b; } u;
u.a = 5;
╭───────┬───────╮
| int a │ int b │
├───────┼───────┤
|  0th  │  0th  │
╰───────┴───────╯
```

```java
address global-ram 0 > u
0 > a
0 > b
(u.add a).write 5
```

### Functions

In a similar way we deal with function call, we calculate the necessary space for arguments (`param-start` and `param-size`) and local variables in `global` for each function call. The variable `r` will be "pushed" to `global` and accessible by the code inside the function `foo` by the 0th position with local offset. The local variable `x` will also be pushed to the `global` and will be accessible by the 4th with local offset, because the length of `int` is four. 
Also we use separate copy of `ram` named `return` for storing function return result. Here, we are trying to simulate the bevaviour of a typical C compiler. The declaration of `foo` and its execution may look like this:

```c
double pi = 3.14;
void circle(int r) {
  double x = 2 * pi * r;
  return x;
}
circle(10);
╭──────────┬───────┬──────────╮
| double z │ int r │ double x │ // variables in global
├──────────┼───────┼──────────┤
|    0th   │  8th  │   12th   │ // start position in global
╰──────────┴───────┴──────────╯
```

```java
address global-ram 0 > pi
[param-start param-size] > circle
  global.read param-start > r
  global.read (add param-start 4) > x
  seq > @
    x.write (2.mul (pi.mul r))
    return.write x

seq
  pi.write 0 3.14
  global.write 8 10 // write 10 to circle arguments stack
  circle 8 4        // arguments stack start from 8th byte and have 4 bytes for r
```

### Function call operators

The function has input variables and local variables. To determine the amount of memory for input variables, we use two parameters in the function description. For the convenience of accessing local variables, we use the bias `local-start` of the local position. To indicate a free position, we use `empty-local-position`. We divide the nested function call into several consecutive calls, the result of which is passed to subsequent calls.

```c
long long func1(long long x) {
  return x - 111;
}

long long func2(long long x) {
  return x - 10;
}

void main() {
  long long a;
  a = func1(func2(5));
  printf("%lld\n", a);
}  
```

```java
[param-start param-size] > func1
  add param-start param-size > local-start
  add local-start 0 > empty-local-position
  address global-ram (param-start.add 0) > x
  seq > @
    return.write (x.sub 111)
    TRUE

[param-start param-size] > func2
  add param-start param-size > local-start
  add local-start 0 > empty-local-position
  address global-ram (param-start.add 0) > x
  seq > @
    return.write (x.sub 10)
    TRUE

[] > main
  seq > @
    a.write // write func1 return in a
      seq
        write // write func2 return in temp place
          address global-ram (add empty-local-position 0) 
          seq
            write // write 5 to func2 arguments stack
              address global-ram (add empty-local-position 0)
              5
            ^.func2 empty-local-position 8
            return
        ^.func1 empty-local-position 8
        return
    printf "%d\n" a
```

### Single return

For the simplest non-nested return, we simply generate a record of the result in a separate ram memory object. Further, other functions can read the result from there.

```c
... bar(){
  return result;
} 
```

```java
ram 1024 > return 
[param-start] > bar
  seq > @
    return.write result
```

### Pointers

C code may get an address of a variable, which is either in stack or in global memory:

```c
int f = 7;
void bar() {
  int t = 42;
  int* p = &t; // local scope
  *p = 500;    // write from local scope to local
  p = &f;      // global scope
  *p = 500;    // write from local scope to global
}
╭───────┬───────┬────────╮
| int f │ int t │ int* p │ // variables in global
├───────┼───────┼────────┤
|  0th  │  4th  │   8th  │ // start position in global
╰───────┴───────┴────────╯
```

However, as in C, our variables are located in `global` and have absolute address.
The object `param-start` provided as an argument to EO object `bar` is a calculated offset in `global` addressing the beginning of the frame for function call. Thus, `&t` would return `param-start + 0`, while `&f` would be just `0`:

```java
[param-start] > bar
  global.write
    8               // int* p
    param-start     // &t -> function offset position in global space
  global.write
    8
    0               // &f -> address of f in global

seq > @
  bar 4
```

### External links

To compile files with any external links, we use the following solution:

In the file where the external call is used, we generate the following alias

```c
#include <string>
strncpy(str2, str1, 8);
```

```java
+alias c2eo.external.strcpy
strncpy str2 st1 8
```

Сreating a file of the same name by the specified alias with an empty implementation

```java
+package c2eo.external

[args...] > strncpy
  TRUE > @
```

### if-else

In EO, we have an analog of an [if-else](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/if.eo) object, so we just convert without any significant changes.

```c
if (condition) {
  ...
}
else {
  ...
}
```

```java
if
  condition
  seq
    ...
    TRUE
  seq // else
    ...
    TRUE
```

### while-do

In EO, we have an analog of the [while](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/while.eo) object, so we just convert without any significant changes.

```c
while (condition) {
  ...
}
```

```java
while
  condition
  seq
    ...
    TRUE
```

For do-while, we need to create the [do-while](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/do-while.eo) object.

```c
do {
  ...
} while (condition)
```

```java
do-while
  condition
  seq
    ...
    TRUE
```

### for

For "for" we can also use the while objecе by placing the "for" expression blocks in different places:

```c
for (initialization, condition, modification) {
  body
}
```

```java
seq // initialization
  ...
  TRUE
while
  condition
  seq 
    body // first
    modification // last
    TRUE
```

example:

```c
for (i=0; i<x; i++) {
  printf("%d\n",i);
}
```

```java
i.write 0
while
  i.less < x
  seq 
    printf "%d\n" i
    i.write (i.add 1)
    TRUE
```

### Operators

The table of all C operators and similar objects in the EO.

С|EO
-|-
+|[add](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/add.eo)
-|[sub](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/sub.eo)
*|[mul](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/mul.eo)
/|[div](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/div.eo)
=|[write](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/write.eo)
%|[mod](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/mod.eo)
==|[eq](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/eq.eo)
!=|[neq](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/neq.eo)
<|[less](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/less.eo)
<=|[leq](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/leq.eo)
\>|[greater](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/greater.eo)
\>=|[geq](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/geq.eo)
&&|[and](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/and.eo)
\|\||[or](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/or.eo)
!|[not](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/not.eo)
-x|[neg](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/neg.eo)
++x|[pre-inc-\<type>](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/pre-inc-int64.eo)
x++|[post-inc-\<type>](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/post-inc-int64.eo)
--x|[pre-dec-\<type>](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/pre-dec-int64.eo)
x--|[post-dec-\<type>](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/post-dec-int64.eo)
(double)|[as-float64](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/as-float64.eo)
(long long int)|[as-int64](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/as-int64.eo)

### In progress

### Global initialization

Writing to memory specifying the correct type of variable.

```c
int a = 3;
```

```java
address global-ram 0 > a
a.write-as-int32 3
```

### Calling functions with local variables from other functions

For such a call, it is necessary to calculate the free space for local variables and generate them.

```c
int foo1 () {
  int b;
  foo2(a);
  retun 0;
}

int foo2 () {
  int b;
  return 0;
}

void main () {
  foo1()
}
```

### Basic types

Some types are not yet implemented due to problems with working with bytes in the EO.

```c
char a = '1';
short int b = 2;
long int c = 3;
float d = 4.0;
unsigned int e = 5;
```

### Bit operators

Some operators are not yet implemented due to problems with working with bytes in the EO.

C|EO
-|-
&|[bit-and](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/bit-and.eo)
\||[bit-or](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/bit-or.eo)
^|[bit-xor](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/bit-xor.eo)
~|[bit-not](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/bit-not.eo)
<<|[shift-right](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/shift-right.eo)
\>>|[shift-left](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/shift-left.eo)

### Not implemented

### Multiple return

To solve the multiple return problem, we can use the [goto](https://github.com/objectionary/eo/blob/master/eo-runtime/src/main/eo/org/eolang/gray/goto.eo) object in eo. By wrapping the entire function in a similar object, we can interrupt its execution at any time. To do this, you just need to generate a g.forward call for each return.

```c
function {
  ...
  return <result_1>;
  ...
  return <result_2>;
  ...
  return <result_3>;
}
```

```java
[] > function
  goto > @
    [g]
      seq > @
        ...
        return.write <result_1>
        g.forward
        ...
        return.write <result_2>
        g.forward
        ...
        return.write <result_3>
        g.forward
```

### Switch case default

We can convert such simple switch statement to the nested [if](https://github.com/polystat/c2eo/tree/master/result/eo/c2eo/coperators/if.eo) object.

```c
switch (expression) {
  case const1:
    break;
  case const2: case const3:
    break;
  default:
    break;
}
```

```java
if
  expression.eq const1
  seq
    ...
    True
  if
    or
      expression.eq const2
      expression.eq const3
    seq
      ...
      True
    seq // default
      ...
      True
}
```

### Const

We will replace all calls to the constant with its value.

```c
const int a = 3;
if (a == 10) {
  ...
}
```

```java
if
  3.eq 10
  seq
    ...
    True
  seq
    True
```

### Enums

We can work with enumerated types as well as with constants and substitute numeric values instead of names.

```c
enum State {Working = 1, Failed = 0};
if (10 == Working) {
  ...
}
```

```java
if
  10.eq 1
  seq
    ...
    True
  seq
    True
```

### Break
---

With [goto](https://github.com/objectionary/eo/blob/master/eo-runtime/src/main/eo/org/eolang/gray/goto.eo) object we can transofrm any number of breaks in cycle to g.forward call.

```c
while (condition) {
  ...
  break;
  ...
}
```

```java
goto
  [g]
    while > @
      condition
      seq
        ...
        g.forward
        ...
```

### Continue

With [goto](https://github.com/objectionary/eo/blob/master/eo-runtime/src/main/eo/org/eolang/gray/goto.eo) object we can transofrm any number of continue in cycle to g.backward call.

```c
while (condition) {
  ...
  continue;
  ...
}
```

```java
goto
  [g]
    while > @
      condition
      seq
        ...
        g.backward
        ...
```

### Goto and labels

Current [goto](https://github.com/objectionary/eo/blob/master/eo-runtime/src/main/eo/org/eolang/gray/goto.eo) object can replace continue and break, but goto in C can jump anywhere in function body.

```c
int program() {
  _0:
    if(lc==KWBEGIN) {nxl(); goto _1;}
    return 0;
  _1:
    if(decl()) {goto _2;}
    if(oper()) {goto _2;}
    er(4); return 0;
  _2:
    if(lc==PCL) {nxl(); goto _1;}
    if(lc==KWEND) {nxl(); goto _end;}
    er(4); return 0;
  _end:
    return 1;
}
```

### Calling functions with variable number of arguments

Also in C it is possible to call a function with a variable number of arguments.

```c
double average(int num,...) {
  va_list valist;
  double sum = 0.0;
  int i;
  /* initialize valist for num number of arguments */
  va_start(valist, num);
  /* access all the arguments assigned to valist */
  for (i = 0; i < num; i++) {
    sum += va_arg(valist, int);
  }
  /* clean memory reserved for valist */
  va_end(valist);
  return sum / num;
}

int main() {
  printf("Average of 1, 2, 3, 4 = %f\n", average(4,  1, 2, 3, 4));
  printf("Average of 1, 2, 3 = %f\n",    average(3,  1, 2, 3));
}
```

</details>
