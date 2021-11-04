# A set of scripts for running tests

*Read this in other languages: [Russian](readme.ru.md)*

You have to make sure you have completed this [guide](https://github.com/polystat/c2eo#2-build-llvm--clang)

## Directory Hierarchy

Tests are placed in the `tests` directory. Each test, a C file, is in a separate subdirectory. The program file name is the same as the directory name and has the extension `.c`. During testing, additional data with test information is created in these test directories.

The `configuration` directory contains files used for sample testing. Each file contains a list of test subdirectories located in the `test` directory. One subdirectory name on one line.

## Run scripts

### Run only one test

`python3 testOne.py <subdirectory name>`

Only one test is executed, located in the specified subdirectory.

### Run all tests

`python3 testAll.py`

All tests located in subdirectories of the `test` directory are run. Runs `testOne.py` in a loop to bypass all tests.

### Selectively run tests

`python3 testSelected.py <file-of-configuration>`

The tests located in the specified configuration file in the `configuration` directory are run. The number of configuration files and the number of tests in them can be arbitrary. To bypass the tests specified in the configuration, it runs `testOne.py` in a loop.

### Clearing directories for all tests

`python3 cleanAll.py`

Helper utility that runs to clean up test directories. All that remains is the file with the test code in the C language. It can be used for data synchronization, when the garbage needs to be quickly picked up. When running tests, each directory is pre-cleared without using this utility.

## Helper scripts

### Generating an intermediate test file

`splitter.py <input-file-path output-file-path>`

It is intended for setting a comment on the lines of the test C-file starting with the `#` character and the `printf` function, which allows you to remove the I / O libraries from the test script, which are used when compiling and running the reference C-code. As a result, the file `* -eo.c` is generated from the file` * .c` and used by the transpiler. This script is run inside `testOne.py`.