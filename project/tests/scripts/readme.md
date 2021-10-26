# Testing:

## to run all tests:
```bash
python3 utests.py -v
```

## to run specific test:
```bash
python -m unittest <tests_file_name>.<TestClass>.<testname>
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

### example:
```bash
python -m unittest global_tests.GlobalTestCase.test_long_long_min
```
