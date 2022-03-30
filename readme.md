<img src="https://www.yegor256.com/images/books/elegant-objects/cactus.svg" height="92px" /> 

[![build-project](https://github.com/polystat/c2eo/actions/workflows/build.yml/badge.svg)](https://github.com/polystat/c2eo/actions/workflows/build.yml)
[![License](https://img.shields.io/badge/license-MIT-white.svg)](https://github.com/polystat/c2eo/blob/master/license.txt)
[![Lines of code](https://tokei.rs/b1/github/polystat/c2eo)](https://tokei.rs/b1/github/polystat/c2eo)
[![Hits-of-Code](https://hitsofcode.com/github/polystat/c2eo?branch=master)](https://hitsofcode.com/github/polystat/c2eo/view?branch=master)

This is a experimental translator of C programs to [EO](https://www.eolang.org) programs.

## How to Use

Assuming, you are on [Ubuntu 20+](https://ubuntu.com/download):

```bash
$ sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F7C91591CC543ECA
$ sudo add-apt-repository 'deb http://c2eo.polystat.org/debian/ c2eo-rep non-free main contrib'
$ sudo apt install c2eo
```

Then, just run:

```bash
$ c2eo <c-file-name> <eo-file-name>
```

 We do not support the utility for other distributions and operating systems yet. However, you can try to build the project from source at your own risk. 

## How to Contribute

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

It is assumed that the `llvm-clang` is located in the same dir as the `c2eo`. If your `llvm-clang` is in different place, set the path in that [line](https://github.com/polystat/c2eo/blob/3f687397f245658ee4ec14583b20fe114c873b15/project/src/transpiler/CMakeLists.txt#L7). Then:

```bash
$ cd ./c2eo/project/build
$ cmake ..
$ make
``` 

Then, run tests:

```bash
$ cd ../scripts
$ ./transpile_с2eo.py <path-to-dir-with-C-program>
```

Now the generated project is in `result/`.

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

</p>

<details>
  <summary>3. Project structure</summary>

    .
    ├── collection
    │   ├── c-sources
    │   └── eo-sources
    ├── doc 
    ├── llvm-clang 
    ├── project 
    │   ├── assembly
    │   ├── bin
    │   │   └── c2eo
    │   ├── build
    │   ├── scripts
    │   ├── lib
    │   ├── src
    │   │   └── transpiler
    │   ├── tests
    │   └── CMakeLists.txt
    └── result
        ├── pom.xml
        ├── README.md
        ├── run.sh
        └── eo
            └── c2eo
                └── src

* ### collection
  The `collection` directory contains source codes for programs in the C and EO programming languages, which are supposed to be used both for integration testing of the transpiler and for testing possible options for transforming into EO. C programs are located in the `c-sources` subdirectory. They form data sets that allow assessing the performance of the transpiler being developed. The subdirectory `eo-sources` contains programs on EO, which are used to analyze various variants of code generation, as well as to analyze the possibility of transforming programs from C to EO.

* ### doc
  The `doc` directory contains the documentation generated during the work on the project.


* ### llvm-clang
  The `llvm-clang` directory is for storing the build of the llvm project. It is assumed that this assembly will be formed at the level of binary codes for a specific autonomous implementation (docker, VM) and will not change subsequently. It is unlikely that during the development of the project it is worth switching to a more recent version of llvm without special reason. Being within the project will allow it to be distributed along with the result of the work. At the same time, you can look at and throw away all unnecessary things that are not needed for the project, thereby reducing 8 gigabytes to a more acceptable value.

* ### project
  The `project` directory contains everything that is the result of development. The following directories are currently being viewed in it:

  * #### assembly
    A directory intended for storing intermediate results, as well as the final result of the transpiler operation. The final file `global.eo` is formed from individual intermediate files in it, which is then copied (sent) to the corresponding subdirectory of the` result` directory.

  * #### bin
    The directory in which all executable files and scripts that provide the transpiler operation are grouped.

  * #### build
    The directory for building the project. It is supposed to build the project using `cmake`. In this regard, the hierarchical organization of files is used in each of the subprojects that provide the execution of individual functions, if necessary, a separate` CMakeLists.txt` file should be placed. Also, the root file `CMakeLists.txt` is located in the` project` directory.

  * #### lib
    A directory intended for storing static and (or) dynamic libraries generated during project creation, if any.

  * #### src
    The src directory is key for development. At this stage, two main projects are viewed, placed in the respective catalogs. In general, its content is likely to change.

    * ##### transpiler
      A transpiler that parses the AST of a single compilation unit and produces EO objects as output. These objects are located in the assembly directory and split over two files. One collects all objects corresponding to global artifacts, and the other - static. Taking into account the specifics of the AST analysis, this project is implemented in C ++.

    * ##### collector
      The directory in which the `global.eo` file collector is developed. In general, after the transpiler has processed all compilation units, it links from multiple files containing both static and global objects into a single object file on EO. The development of this program can be carried out in the scripting language.

    * ##### launcher
      A directory containing a program that runs the transpiler several times according to the number of compilation units, and then transfers control to the collector of the resulting individual files into the monolith. After completing the build, this program transfers the generated file `global.eo` to the directory` result / eo / c2eo / src`.

  * #### [tests](./project/tests/scripts)
    A directory with various test programs and data that check the functionality of the code being developed.

* ### result
  Directory for storing data used by the EO compiler. It contains information about the project on EO, updated every time the project of this compiler is updated.

  * #### README.md
    Description of the compiled project, which is formed by the c2eo developers and practically does not change (it can only be corrected);

  * #### run.sh
    A script that launches the compiled application;

  * #### eo
    A directory containing the libraries used, written in EO to support C artifacts, as well as EO sources generated by the transpiler or generated by hand. Within the `eo` directory, the structure is formed by subdirectories. Directly under `eo` is the` c2eo` directory, which defines the common name of the package. It contains:

    * ##### app.eo
      Responsible for launching the application (it is written manually and does not change);

    * ##### ctypes
      A directory that essentially defines some library of objects written in EO and designed to simulate various artifacts of the C language;

    * ##### src
      The directory where the file `global.eo` is written with objects generated by the transpiler during the analysis of a C program (it contains a `global` object, in which all artifacts are collected in the form of corresponding objects).

The formation of the `global.eo` file is essentially the main task of the transpiler and provides, by assembling many units, the compilation of the source C program.

&nbsp;
## Placing a program on EO obtained during transpilation
The presented structure became possible due to the use of initial initialization of objects that mimic C variables.

Transpilation is carried out separately for each compilation unit, which generates two files at the output:

* a file with a description of all global objects, which include abstract objects obtained by transforming abstract data types, global variables, global function descriptions;

* a file with a description of all static objects that are transformed from the descriptions of static variables and functions located in the global space, static variables located inside functions.

These two files are a basic stub for further build after transpilation of all compilation units of the project. The assembly itself at the moment consists in the formation of a common file in the EO programming language. It forms a global object `global`, which contains all objects obtained as a result of compilation of abstract data types, external variables, external functions, as well as objects that are obtained from files describing static objects.

The number of static objects is determined by the number of files with static artifacts. Placing all data in a single object `global` allows you to easily provide access both from global objects to their static data and from external static objects to global data. The compiler of this file can, in principle, be a separate program implemented in any convenient programming language.

If there is a `main` function in one of the compilation units, it is converted to the corresponding object of the global space. And immediately after its description, a description of its launch follows. The function can be located anywhere in the global object.

In general, the order in which a file with global objects and static objects is assembled is irrelevant.

The presented scheme provides complete autonomy for the formation of the program on EO. The object that launches the application contains only the dateization of the global object. It does not change, remaining constant regardless of the transpiled project.

</details>

</p>

<details>
    <summary>4. About working with the project</summary>

&nbsp;
## Executable transpiler file and scripts for building the program on EO

---
## Directory Hierarchy

These files are located in the `project/bin` directory

&nbsp;
## Run scripts

### Launching the program builder on EO

`python3 launcher.py <file-of-c-program>`

The `c2eo` transpiler is launched, then the `collector.py` script is launched. As a result of processing the source file in the C language, the file `global.eo` on EO is generated, which is transferred to the directory `result/eo/c2eo/src/` of the project on EO.

&nbsp;
## Support scripts and programs
---
### Transpilation and formation of intermediate files

`c2eo <file-of-c-program> <name>`

Executable native code launched from the launcher. Carries out transpilation of a C program with the formation of intermediate files containing separately external (global) and static artifacts with the extensions `*.glob` and `*.stat`, respectively. The files are created in the `project/assembly` directory. The transmitted name is generated by the launcher and sets the names for intermediate files. It can also run autonomously.

### Generating the global.eo file

`collector`

Based on intermediate files located in the `project/assembly` directory, it also creates a program file on EO `global.eo`. Launched from the launcher.

---
</details>

</p>
