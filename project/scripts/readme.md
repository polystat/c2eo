# Scripts for building the program on EO

*Read in another language: [Russian](readme.ru.md)*

Make sure you completely follow the main [guide](https://github.com/polystat/c2eo#2-build-llvm--clang)

All scripts are run from the `c2eo/project/scripts` folder

## Possible commands

1. Start testing
    ```bash
    # By default, the path "path_to_tests" and the "config" configuration in settings.yml will be taken, you can set other values in settings.yml

    $ ./test.py


    # You can specify a different path at startup

    $ ./test.py ../tests/nkchuykin


    # You can specify a different path and configuration at startup

    $ ./test.py ../tests/nkchuykin without_struct
    ```

    <details>
    <summary> <bold> More information ... </bold> </summary>
    During testing, the EO version is checked, then <code> c2eo </code> is collected, the test results are cleared, files are searched in C, and their subsequent transpilation using <code> c2eo </code>. Then the resulting files are collected into files on the EO. After that, the project is transferred to the EO, and the assembly of the project is started. Testing and comparison of the results of files in C and EO is carried out.
    <p> </p>

    Tests are placed in the <code> tests </code> directory. Each test, a C file, is in a separate subdirectory. The program file name is the same as the directory name and has the .c extension. During testing, additional data with test information is created in these test directories.

    The <code> scripts/data/config </code> directory contains files used for custom testing. Each file contains a list of lines, one of which must contain the path to the file. If you put a! Sign at the beginning of a line, then there should not be a single line from this list in the path to the file.
    </details>
    &nbsp;

1. Transpilation and assembly of all C-files found in this directory

    ```bash
    $ ./transpile_c2eo.py ../tests/nchuikin
    ```

1. Clear all the entire catalog, leaving only C-files (without -o at the end)

    ```bash
    # By default, the path "path_to_tests" will be taken, you can set a different value in settings.yml
    
    $ ./clean_before_transpilation.py


    # You can specify a different path and configuration at startup

    $ ./clean_before_transpilation.py ../tests/nkchuykin
    ```

1. Build EO project `../../ result`

    ```bash
    $ ./build_eo.py
    ```

1. Build c2eo

    ```bash
    # By default, the path "path_to_c2eo" will be taken, you can set a different value in settings.yml
    
    $ ./build_c2eo.py


    # You can specify a different path at startup

    $ ./build_c2eo.py ../my_dir/bin
    ```

1. Build all the `.glob` and `.stat` files into the `.eo` files

    ```bash
    # By default, the path "path_to_assembly" will be taken, you can set a different value in settings.yml
    
    $ ./collect_transpiled_code.py
    ```

1. Update EO version

    ```bash
    $ ./update_eo_version.py
    ```

1. `settings.py` - script with access to project settings

1. `tools.py` - script with useful functions
