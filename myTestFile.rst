Work plan for the first stage
======================================

Stage description
----------------------

The first stage's terms of execution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

28.07.2021-28.10.2021

Stage goal
~~~~~~~~~~~~~~~~~~~~~~~~

A translator that can convert simple (base) C programming language types and elementary operations with them to EOLANG objects.

Stage content (tasks to be solved)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Analysis of transformation methods and the LLVM+Clang system.
2. Determination of the subset of the C programming language used for transformation during translation to EOLANG objects.
3. The translator prototype that implements the transformation of simple (base) data types and operations with them.

Results to be obtained under the contract
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. The subset of the C programming language used for transformation.
2. A translator written as additional modules for Clang
3. 50+ automated tests.
4. Technical report with a brief description of the implementation details.


Weekly results of work for the first stage
------------------------------------------------------

Week 1 (28.07.2021 - 05.08.2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. The Beginning of  analyze the LLVM Clang tools and libraries used to implement the transpiler. Demonstration examples testing. Identifying opportunities toget the required information for transformation from C programming language.
#. Collecting of information on the EOLANG programming language to form a semantic model.

Week 2 (08/06/2021 - 08/12/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Continued analysis of tools. Adding validation of global integer variables to demo examples. Analysis of obtaining information from an abstract syntax tree necessary for generating the corresponding EOLANG objects.
#. Start of collecting demo examples in the C programming language, which are used in the future to form test coverage and demonstrate the operation of the transpiler.

Week 3 (08/13/2021 - 08/19/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Analysis of the features of the EOLANG language. Selecting of atomic objects and comparison of their semantics with the corresponding constructs of the C programming language.
#. Test cases on EOLANG.
#. An initial set of C examples

Week 4 (08/20/2021 - 08/26/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Analysis of parameters used to describe global variables in C programming language. Comparison of global variable declarations with corresponding EOLANG objects. Formation of a correspondence table.
#. EOLANG test cases using global objects.
#. Test cases describing different variants of global integer variables.
#. Preliminary description of the semantic model of the EOLANG language.

Week 5 (08/27/2021 - 09/02/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. The objects that provide mapping of global integer variables to EOLANG. Testing of these objects in the EOLANG programming language.
#. EOLANG test cases using global objects to represent data of various types.
#. Test suites describing different variants of global variables of various types.
#. Representation of the semantic model EOLANG in C++ (initial version).

Week 6 (09/03/2021 - 09/09/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Transpiler code for generating of the integer variable models on EOLANG.
#. Test suites that test the generation of models of integer variables.
#. Representation of the semantic model EOLANG in C++ (continued).

Week 7 (09/10/2021 - 09/16/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Transpiler code for generating of models of the base type variables  in EOLANG.
#. Test cases that check the generation of models of the base type variables.
#. Representation of the EOLANG semantic model in C ++ (the end).

Week 8 (17.09.2021 - 23.09.2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. An EOLANG object that simulates a function without parameters.
#. Test suites for checking the EOLANG semantic model.
#. Adding transpiler fragments that generate code for basic operations.
#. Test suites for testing basic operations.

Week 9 (09.24.2021 - 09.30.2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. The EOLANG object that simulates a function without parameters and with local objects.
#. Test suites for checking the EOLANG semantic model.
#. Adding transpiler fragments that generate code for functions without parameters using global variables.
#. Test cases for testing basic operations with global variables.

Week 10 (10/01/2021 - 10/07/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. An EOLANG object that simulates a function with parameters and local objects.
#. Test suites for checking the EOLANG semantic model.
#. Adding transpiler fragments that generate code for functions without parameters but using global variables.
#. Test cases for testing basic operations with local and global variables.

Week 11 (10/08/2021 - 10/14/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. An EOLANG object that simulates a function with parameters, as well as local and global objects.
#. Test suites for checking the EOLANG semantic model.
#. Adding transpiler fragments that generate code for functions with parameters, local and global variables.
#. Test cases for testing basic operations with parameters, local and global variables.

Week 12 (10/15/2021 - 10/21/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Debugging and testing of transpiler the prototype.

Week 13 (10/22/2021 - 10/28/2021)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Demonstration prototype of the transpiler.
#. Report

