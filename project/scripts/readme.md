# Scripts for automating work with the c2eo tranpsiler

Make sure you completely follow the main [guide](https://github.com/polystat/c2eo#How-to-contribute)

All scripts are run from the `project/scripts` folder

### 1. build_c2eo.py

```bash
usage: build_c2eo.py [-h] [-p PATH]

the script for building c2eo in the specified directory

options:
  -h, --help            show this help message and exit
  -p PATH, --path_to_c2eo_build PATH
                        the relative path from the scripts folder to the build folder (default: ../build)
```

### 2. build_eo.py

```bash
usage: build_eo.py [-h]

the script for building eo project in the result dir

options:
  -h, --help  show this help message and exit
```

### 3. c2eo-all.py

```bash
usage: c2eo-all.py <path_to_files>

the script for single-threaded launch of c2eo without formatting the output to the console for all c files
```

### 4. clang-tidy.py

```bash
usage: clang_tidy.py [-h] [-p PATH]

script for checking code files using Clang-Tidy

options:
  -h, --help            show this help message and exit
  -p PATH, --path_to_code_files PATH
                        the relative path from the scripts folder to the code files folder (default: ../src/transpiler)
```

### 5. clean_before_transpilation.py

```bash
usage: clean_before_transpilation.py [-h] [-p PATH] [-c CLEANING_PATTERNS]

the script for cleaning the folder from c2eo transpiled files

options:
  -h, --help            show this help message and exit
  -p PATH, --path_to_files PATH
                        the relative path from the scripts folder to the c files folder (default: ../tests/main)
  -c CLEANING_PATTERNS, --cleaning_patterns CLEANING_PATTERNS
                        the string of file patterns for cleaning (default: *.alias *-eo.c *.eo *.out *.log *.txt *.i)
```

### 6. code_lines.py

```bash
usage: code_lines.py <path_to_files>

show code lines statistics for the dir 
```

### 7. compile.py

```bash
usage: compile.py [-h] [-p PATH] [-s FILE_NAME] [-n] [-c]

the script for compiling transpiled files from C to EO

options:
  -h, --help            show this help message and exit
  -p PATH, --path_to_files PATH
                        the relative path from the scripts folder to the c files folder (default: None)
  -s FILE_NAME, --skips_file_name FILE_NAME
                        the name of the file with a set of skips for tests (default: )
  -n, --not_prepare_c_code
                        the script will not change the c code in the input files (default: False)
  -c, --codecov         the script will generate codecov files (default: False)
```

### 8. csmith.py

```bash
usage: csmith.py [-h] [-c COUNT] PATH

the script for generating csmith testsuite for c2eo transpiler

positional arguments:
  PATH                  the relative path from the scripts folder to the generating folder

options:
  -h, --help            show this help message and exit
  -c COUNT, --count_of_files COUNT
                        the count of generating files (default: 1)
```

### 9. eo_execute.py

```bash
usage: eo_execute.py [-h] [-p PATH]

the script for executing eo files

options:
  -h, --help            show this help message and exit
  -p PATH, --path_to_eo_file PATH
                        the relative path from the scripts folder to the eo file (default: None)
```

### 10. settings.py

```
the script for storing and working with project settings
```

### 10. test.py

```bash
usage: test.py [-h] [-p PATH] [-s FILE_NAME] [-n] [-c]

the script for testing the correctness of the execution of translated files from C to EO

options:
  -h, --help            show this help message and exit
  -p PATH, --path_to_tests PATH
                        the relative path from the scripts folder to the tests folder (default: ../tests/main)
  -s FILE_NAME, --skips_file_name FILE_NAME
                        the name of the file with a set of skips for tests (default: None)
  -n, --not_prepare_c_code
                        the script will not change the c code in the input files (default: False)
  -c, --codecov         the script will generate codecov files (default: False)
```

### 11. tools.py

```bash
the script with useful functions for other scripts to work
```

### 12. transpile.py

```bash
usage: transpile.py [-h] [-s FILE_NAME] [-n] [-c] PATH

the script for translating C files to the EO files

positional arguments:
  PATH                  the relative path from the scripts folder to the folder with c files

options:
  -h, --help            show this help message and exit
  -s FILE_NAME, --skips_file_name FILE_NAME
                        the name of the file with a set of skips for transpile (default: )
  -n, --not_prepare_c_code
                        the script will not change the c code in the input files (default: False)
  -c, --codecov         the script will generate codecov files (default: False)
```
