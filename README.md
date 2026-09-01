<img src="https://www.yegor256.com/images/books/elegant-objects/cactus.svg" height="92px" /> 

[![build](https://img.shields.io/github/actions/workflow/status/polystat/c2eo/build.yml?branch=master)](https://github.com/polystat/c2eo/actions/workflows/build.yml?query=event%3Aschedule)
[![version](https://img.shields.io/github/v/release/polystat/c2eo?include_prereleases)](https://github.com/polystat/c2eo/releases/latest)
[![codecov](https://codecov.io/gh/polystat/c2eo/branch/master/graph/badge.svg)](https://codecov.io/gh/polystat/c2eo)
![Lines of code](https://tokei.rs/b1/github/polystat/c2eo)
[![Hits-of-Code](https://hitsofcode.com/github/polystat/c2eo)](https://hitsofcode.com/view/github/polystat/c2eo)
[![license](https://img.shields.io/github/license/polystat/c2eo?color=e6e6e6)](https://github.com/polystat/c2eo/blob/master/license.txt)

This is a experimental translator of C ([ISO/IEC 9899:2018](https://www.iso.org/standard/74528.htm)) programs to [EO](https://www.eolang.org) programs.

## How to Use

Assuming, you are on [Ubuntu 22.04+](https://ubuntu.com/download):

```bash
$ apt update
$ apt install -y software-properties-common
$ apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F7C91591CC543ECA
$ add-apt-repository 'deb http://c2eo.polystat.org/debian/ c2eo-rep non-free main contrib'
$ apt-get install -y clang
$ apt-get install -y c2eo
```

Then, just run:

```bash
$ c2eo <path-to-c-file-name> <eo-file-name>.eo
```

You can also use [yegor256/c2eo](https://hub.docker.com/r/yegor256/c2eo/tags)
image via [Docker](https://docs.docker.com/get-docker/):

```bash
$ docker run -v $(pwd):/eo yegor256/c2eo:<tag> hello.c hello.eo
```

Assuming you have `hello.c` in the current directory, the `hello.eo` will be created next to it.

We do not support the utility for other distributions and operating systems yet.
However, you can try to build the project from source at your own risk.

## How to Contribute

Again, we recommend [Ubuntu 22.04+](https://ubuntu.com/download) and you will need
[wget 1.21+](https://www.tecmint.com/install-wget-in-linux/), 
[tar 1.30+](https://www.tecmint.com/install-tar-in-centos-rhel-and-fedora/), 
[git 2.32.+](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git),
[cmake 3.18+](https://cmake.org/download/), 
[gcc 11.2.+](http://mirror.linux-ia64.org/gnu/gcc/releases/), 
[g++ 11.2.+](https://pkgs.org/download/g++), 
[ninja-build 1.10.1+](https://ninja-build.org/), 
[clang 14.0.0+](https://releases.llvm.org/14.0.0/tools/clang/docs/)
and
[python3 3.10.0+](https://www.python.org/downloads/).
You will also need requirements for [the EO project](https://github.com/objectionary/eo) ([Maven 3.3+](https://maven.apache.org) and Java 8+)

Then, you need to install [GTest 1.12.1+](https://github.com/google/googletest)

```bash
$ apt install libgtest-dev googletest
$ cd /usr/src/googletest
$ cmake .
$ make
$ lib
$ cp *.a /usr/local/lib
```

After that, you need to install [LLVM/Clang 12.0.1](https://github.com/llvm/llvm-project/releases/tag/llvmorg-12.0.1) or you may use an alternative way below this code:

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
$ apt install megatools
$ megadl 'https://mega.nz/#!cZ9WQCqB!z713CuC-GNFQAXIxZwZxI05zOH4FAOpwYHEElgOZflA'
$ tar -xvf llvm-clang.tar.gz
```

It is assumed that the `llvm-clang` dir is located in the `c2eo` dir. If your `llvm-clang` is in different place, set the path in that [line](https://github.com/polystat/c2eo/blob/master/src/transpiler/CMakeLists.txt#L6).

Formally speaking, this is where the preparation can be completed. However, in order to fully work with the project, testing and executing the translated code, you need to study the [EO compiler project](https://github.com/objectionary/eo) and fulfill its necessary requirements. After that, it will be possible to proceed with further steps.

### Making changes

All sources files of transpiler are located in `src/transpiler`. The transpiler's work begins with the code from the source file `src/transpiler/main.cpp`. Аfter making changes in these files, we will need to rebuild the executable file `c2eo`. To do this, you need to go to the repository root. For the first time, create the `build` folder:

```bash
$ mkdir build
```

then go to the `build` folder and run the following commands:

```bash
$ cmake ..
$ make
``` 

As you have already noticed, the project is being built in the `build` folder. The result of this build is the `c2eo` file in `bin`. Now you have a transpiler and you can convert programs from C to EO. Just run:

```bash
$ ./c2eo <path-to-c-file-name> <eo-file-name>.eo
# ./c2eo ../some_dir/example.c example.eo
```

### Checking before creating PR

Your PR will pass the following checks, so before creating PR run these locally to make sure everything is ok:

1. [clang-format-14](https://pypi.org/project/clang-format/)
```bash
$ clang-format src/transpiler/*.(cpp|h) -i 
```

2. [cpplint](https://github.com/cpplint/cpplint)
```bash
$ cpplint --filter=-runtime/references,-runtime/string,-build/c++11 src/transpiler/** 
```
3. [clang-tidy](https://packages.ubuntu.com/en/bionic/clang-tidy)
```bash
$ cd scripts
$ python3 clang_tidy.py
```
4. [gcc.c-torture](https://github.com/polystat/c2eo/releases/download/0.1.16/gcc.c-torture.tar.gz)
```bash
$ cd scripts
$ python3 transpile.py <your_path_to_the_folder>/gcc.c-torture -s gcc -n
```

5. [c-testcuite](https://github.com/polystat/c2eo/releases/download/0.1.16/c-testcuite.tar.gz)
```bash
$ cd scripts
$ python3 test.py -p <your_path_to_the_folder>/c-testcuite -s testcuite -n
```

6. test
```bash
$ cd scripts
$ python3 test.py -s test
```

7. unit-tests
```bash
$ cd scripts
$ python3 build_c2eo.py
$ cd ../bin/
$ ./unit_tests --gtest_filter=*
```

## How to release

From `scripts/` directory:

```bash
$ python3 update-release.py -h
usage: update-release.py [-h] [--branch BRANCH] [--version VERSION]

Release maker

optional arguments:
  -h, --help         show this help message and exit
  --version VERSION  specify the new version
```

Example

```bash
$ python3 update-release.py --version=0.1.1
```

To use this script, make sure you have the following packages installed:

```bash
$ pip3 install git_config pgpy s3cmd
$ apt install md5deep reprepro gcc cmake dpkg wget tar s3cmd -y
# for the latest version of the cmake package, try:
$ pip3 install cmake
```

Notes:

* Use `.` as a version delimiter.
* This script uses the current date, time, and time zone. Make sure they are configured correctly.
* This script extracts your name and email from `git config`. Make sure you have them.

This script will write automatically generated merges to the changelog file.
You can view an approximate list of changes by running the following command in the terminal:

```bash
$ git log $(git describe --tags --abbrev=0)..HEAD --merges --oneline --format="  * %h %s by %an <%aE>"
```

### Algorithm:

* Build the executable file.
* Create a deb file (basic: [HABR](https://habr.com/ru/post/78094/))
* Create a repository (basic: [UNIXFORUM](https://unixforum.org/viewtopic.php?t=79513))
* Upload a repository tree into the bucket's virtual 'directory'.

<details>
<summary>The following files will be generated</summary>

    $ tree
    .
    ├── c2eo-X.X.X
    │   ├── DEBIAN
    │   │   ├── changelog
    │   │   ├── control
    │   │   ├── copyright
    │   │   └── md5sums
    │   └── usr
    │       ├── bin
    │       │   └── c2eo
    │       └── lib
    │           ├── libclangAnalysis.so
    │           ├── libclangAnalysis.so.12
    │           ├── ...
    │           └── libLLVMTransformUtils.so.12
    ├── c2eo-X.X.X.deb
    ├── readme.md
    ├── repository
    │   ├── conf
    │   │   └── distributions
    │   ├── db
    │   │   ├── checksums.db
    │   │   ├── contents.cache.db
    │   │   ├── packages.db
    │   │   ├── references.db
    │   │   ├── release.caches.db
    │   │   └── version
    │   ├── dists
    │   │   └── c2eo-rep
    │   │       ├── contrib
    │   │       │   ├── binary-amd64
    │   │       │   │   ├── Packages
    │   │       │   │   ├── Packages.gz
    │   │       │   │   └── Release
    │   │       │   ├── binary-i386
    │   │       │   │   ├── Packages
    │   │       │   │   ├── Packages.gz
    │   │       │   │   └── Release
    │   │       │   ├── debian-installer
    │   │       │   │   ├── binary-amd64
    │   │       │   │   │   ├── Packages
    │   │       │   │   │   └── Packages.gz
    │   │       │   │   └── binary-i386
    │   │       │   │       ├── Packages
    │   │       │   │       └── Packages.gz
    │   │       │   └── source
    │   │       │       ├── Release
    │   │       │       └── Sources.gz
    │   │       ├── InRelease
    │   │       ├── main
    │   │       │   ├── binary-amd64
    │   │       │   │   ├── Packages
    │   │       │   │   ├── Packages.gz
    │   │       │   │   └── Release
    │   │       │   ├── binary-i386
    │   │       │   │   ├── Packages
    │   │       │   │   ├── Packages.gz
    │   │       │   │   └── Release
    │   │       │   ├── debian-installer
    │   │       │   │   ├── binary-amd64
    │   │       │   │   │   ├── Packages
    │   │       │   │   │   └── Packages.gz
    │   │       │   │   └── binary-i386
    │   │       │   │       ├── Packages
    │   │       │   │       └── Packages.gz
    │   │       │   └── source
    │   │       │       ├── Release
    │   │       │       └── Sources.gz
    │   │       ├── non-free
    │   │       │   ├── binary-amd64
    │   │       │   │   ├── Packages
    │   │       │   │   ├── Packages.gz
    │   │       │   │   └── Release
    │   │       │   ├── binary-i386
    │   │       │   │   ├── Packages
    │   │       │   │   ├── Packages.gz
    │   │       │   │   └── Release
    │   │       │   ├── debian-installer
    │   │       │   │   ├── binary-amd64
    │   │       │   │   │   ├── Packages
    │   │       │   │   │   └── Packages.gz
    │   │       │   │   └── binary-i386
    │   │       │   │       ├── Packages
    │   │       │   │       └── Packages.gz
    │   │       │   └── source
    │   │       │       ├── Release
    │   │       │       └── Sources.gz
    │   │       ├── Release
    │   │       └── Release.gpg
    │   └── pool
    │       └── main
    │           └── c
    │               └── c2eo
    │                   └── c2eo_X.X.X_all.deb
    ├── todo.sh
    └── update-release.py
    
    35 directories, 120 files
</details>

Then you have to upload `./repository/dists` and `./repository/pool` to [c2eo.polystat.org/](http://c2eo.polystat.org)debian/.
