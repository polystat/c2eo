# Testing:

## to run all tests:
```bash
python3 utests.py -v
```

## to run specific test:
```bash
python -m unittest tests_file_name.TestClass.testname
```

### example:
```bash
python -m unittest global_tests.GlobalTestCase.test_long_long_min
```