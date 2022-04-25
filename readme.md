<img src="https://www.yegor256.com/images/books/elegant-objects/cactus.svg" height="92px" /> 

[![build-project](https://github.com/polystat/c2eo/actions/workflows/build.yml/badge.svg)](https://github.com/polystat/c2eo/actions/workflows/build.yml)
[![License](https://img.shields.io/badge/license-MIT-white.svg)](https://github.com/polystat/c2eo/blob/master/license.txt)
[![Lines of code](https://tokei.rs/b1/github/polystat/c2eo)](https://tokei.rs/b1/github/polystat/c2eo)
[![Hits-of-Code](https://hitsofcode.com/github/polystat/c2eo?branch=master)](https://hitsofcode.com/github/polystat/c2eo/view?branch=master)

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
[cmake](https://cmake.org/download/), 
[gcc](http://mirror.linux-ia64.org/gnu/gcc/releases/), 
[g++](https://pkgs.org/download/g++), 
[ninja-build](https://ninja-build.org/)
and
[python3.+](https://www.python.org/downloads/)

Then, you need LLVM/Clang:

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

This completes the preparations for work, now you can start making changes.

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
$ python3 transpile_c2eo.py

# Single-threaded launch of c2eo without formatting the output to the console for all c files
$ python3 c2eo-all ../tests/main

# Show code lines statistics in this dir 
$ python3 code_lines.py ../tests/main
```

The main tests are in the folder `/project/tests/main`, if they pass, then everything is ok. [Here](./project/scripts/readme.md) you can find more information about scripts.

## Principles of Transpilation from C to EO

C is a _system-level procedural_ programming language with direct access to the underlying hardware architecture elements, such as memory and registers. EO, on the other hand is a _high-level object-oriented_ language. There are a number of non-trivial mechanisms of translating constructs from the former to the latter, which are explained below:

### Direct Memory Access

Let's take the following C code as an example:

```c
double z = 3.14;
```

In EO, we represent the global memory space as a copy of [ram](https://github.com/polystat/c2eo/blob/heap/result/eo/c2eo/system/ram.eo) object, which we call `global`. Thus, the variable `z` would be accessed as a block of 8 bytes inside `ram` at the very begginning, since it's the first variable seen. For example, to change the value of `z` we write 8 bytes to the 0th position of `global`:

```java
ram > global
global.write
  0
  3.14.as-bytes
```

### Functions

```c
void foo(int a) {
  double x = z + a;
  return x;
}
╭──────────┬───────┬──────────╮
| double z │ int a │ double x │ // variables in global
├──────────┼───────┼──────────┤
|    0th   │  8th  │   12th   │ // start position in global
╰──────────┴───────┴──────────╯
```

In a similar way we deal with function call, we calculate the necessary space for arguments (`param-start` and `param-size`) and local variables in `global` for each function call. The variable `a` will be "pushed" to `global` and accessible by the code inside the function `foo` by the 0th position with local offset. The local variable `x` will also be pushed to the `global` and will be accessible by the 4th with local offset, because the length of `int` is four. 
Also we use separate copy of `ram` named `return` for storing function return result. Here, we are trying to simulate the bevaviour of a typical C compiler. The declaration of `foo` and its execution may look like this:

```java
[param-start param-size] > foo
  global.read param-start > a
  global.read (add param-start 4) > x
  seq > @
    x.write
      add
        global.read 0
        a
    return.write x

seq
  global.write 0 55.5
  global.write 8 78322
  foo 8 4
```

### Return

```java

ram 1024 > return 

[param-start] > bar
  seq > @
  ...
  return.write result
  TRUE

seq > @
  bar
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

- In the file where the external call is used, we generate the following alias

  ```c
  #include <string>
  strncpy(str2, str1, 8);
  ```

  ```java
  +alias c2eo.external.strcpy
  strncpy str2 st1 8
  ```

- Сreating a file of the same name by the specified alias with an empty implementation

  ```java
  +package c2eo.external

  [args...] > strncpy
    TRUE > @
  ```

### Arrays

We work with fixed-size arrays in the same way as with variables.

```c
int instanceArray[2] = { 5 6 };
╭─────┬─────╮
|  5  │  6  │
├─────┼─────┤
| 0th │ 4th │
╰─────┴─────╯
```

```java
global.write 0 5.as-bytes
global.write 4 6.as-bytes
```

### Structures

We know the size of structures so we still work as with variables. We can also generate additional objects to access the fields of structures.

```c
struct Rectangle {int x; int y;} rect;
```

```java
address > g-rect
  global-ram
  0
address > g-rect-x
  global-ram
  0
address > g-rect-y
  global-ram
  4
```

### Unions

The size of the union is determined by the nested object with the maximum size. The main feature is that internal objects are located at the beginning of the same address.

```c
struct Rectangle {int x; int y;};
struct Triangle {int a, b,c;};
struct Figure {
    int key;
    union {
        Rectangle r;
        Triangle  t;
} fig;
╭─────────┬───────────┬───────────┬───────────┬───────────┬───────────╮
| int key │ r (int x) │ r (int y) │ t (int a) │ t (int b) │ t (int c) │
├─────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
|   0th   │    4th    │    8th    │    4th    │    8th    │    12th   │
╰─────────┴───────────┴───────────┴───────────┴───────────┴───────────╯
```

```java
address > g-fig
  global-ram
  0
address > g-fig-key
  global-ram
  0
address > g-fig-r-x
  global-ram
  4
address > g-fig-r-y
  global-ram
  8
address > g-fig-t-a
  global-ram
  4
address > g-fig-t-b
  global-ram
  8
address > g-fig-t-c
  global-ram
  12
```

### Enums

We can work with enumerated types as well as with constants and substitute numeric values instead of names.

</details>
