# Tests
Here are collected tests to check the C2EO project

&nbsp;
# Getting started guide

Should work. If it doesn't, [submit an issue](https://github.com/polystat/c2eo/issues),
we will fix it.

&nbsp;
## Step 1. Main guide
You have to make sure you have completed this [guide](https://github.com/polystat/c2eo#2-build-llvm--clang)

&nbsp;
## Step 2. Run tests
There are some variants of running test

&nbsp;
* run all tests:

  ```bash
  $ python3 utests.py -v
  ```
&nbsp;
* run specific test:

    ```bash
    $ python3 -m unittest <tests_file_name>.<TestClass>.<testname>
    ```

    > one of the possible specific tests
    ```bash
    $ python3 -m unittest global_tests.GlobalIntegerTestCase.test_long_long_min
    ```

&nbsp;
* run a combination of tests:

    ```bash
    global_tests / static_tests
    .
    GlobalIntegerTestCase / StaticIntegerTestCase
    GlobalSpecificTestCase / StaticSpecificTestCase
    GlobalFloatTestCase / StaticFloatTestCase
    .
    test_<var_type>_<1/2/3>
    test_<var_type>_<min/max>
    test_<var_type>_<min/max>_overflow
    ```


&nbsp;
# Files description

* ### `utests.py`
  generate and run all sets of tests

* ### `global_tests.py`
  contains classes for different types of global variables
and has functions for each test type

* ### `static_tests.py`
  contains classes for different types of static variables
and has functions for each test type

* ### `system_vars.py`
  contains constants for test and folder's templates

* ### `fun.py`
  runs C compiler, Eolang compiler and compares result 
generates C tests wint prinf

