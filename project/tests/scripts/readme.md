# Functions description
## utests.py
### runs all tests:
```bash
python3 utests.py -v
```

## unittest module: 
### runs specific test:
```bash
python -m unittest <tests_file_name>.<TestClass>.<testname>
```
### example:
```bash
python -m unittest global_tests.GlobalTestCase.test_long_long_min
```
possible tests combinations:
```
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

## Other functions
### global_tests.py
contains classes for different types of global variables
and has functions for each test type
### static_tests.py
contains classes for different types of static variables
and has functions for each test type
### system_vars.py
contains constants for test and folder's templates
### fun.py
runs C compiler, Eolang compiler and compares result 
generates C tests wint prinf
