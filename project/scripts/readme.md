# Scripts for building the program on EO

Make sure you completely follow the main [guide](https://github.com/polystat/c2eo#2-build-llvm--clang)

All scripts are run from the `project/scripts` folder

## Possible commands

1. Start testing
    ```bash
    # By default, the path "path_to_tests" in settings.yml will be taken, you can set other values in settings.yml

    $ python3 test.py


    # You can specify a different path at startup

    $ python3 test.py -p ../tests/nkchuykin


    # You can specify a different path and configuration at startup

    $ python3 test.py -p ../tests/nkchuykin -s test
    ```

    <details>
    <summary> <bold> More information ... </bold> </summary>
    During testing, the EO version is checked, then <code> c2eo </code> is collected, the test results are cleared, files are searched in C, and their subsequent transpilation using <code> c2eo </code>. Then the resulting files are collected into files on the EO. After that, the project is transferred to the EO, and the assembly of the project is started. Testing and comparison of the results of files in C and EO is carried out.
    <p> </p>

    Tests are placed in the <code> tests </code> directory. Each test, a C file, is in a separate subdirectory. The program file name is the same as the directory name and has the .c extension. During testing, additional data with test information is created in these test directories.

    &nbsp;

1. Transpilation and assembly of all C-files found in this directory

    ```bash
    $ python3 transpile.py -p ../tests/nchuikin
    ```

1. Clear all the entire catalog, leaving only C-files (without -o at the end)

    ```bash
    # By default, the path "path_to_tests" will be taken, you can set a different value in settings.yml
    
    $ python3 clean_before_transpilation.py


    # You can specify a different path and configuration at startup

    $ python3 clean_before_transpilation.py -p ../tests/nkchuykin
    ```

1. Build EO project `../../ result`

    ```bash
    $ python3 build_eo.py
    ```

1. Build c2eo

    ```bash
    # By default, the path "path_to_c2eo" will be taken, you can set a different value in settings.yml
    
    $ python3 build_c2eo.py


    # You can specify a different path at startup

    $ python3 build_c2eo.py -p ../my_dir/bin
    ```

1. Single-threaded launch of c2eo without formatting the output to the console for all c files 

    ```bash
    $ python3 c2eo-all.py ../tests/main
    ```

1. Show code lines statistics for this dir 

    ```bash
    $ python3 code_lines.py ../tests/main
    ```

1. Update EO version

    ```bash
    $ python3 update_eo_version.py
    ```

2. `settings.py` - script with access to project settings

3. `tools.py` - script with useful functions
