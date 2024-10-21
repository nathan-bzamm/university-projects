# cps1011-assignment-base-code

## Introduction

This repo contains the necessary files to get you started with the assignment for CPS 1011. The following will be highlighted in the sections below:
  
  1. Getting Started
  2. Repo Structure
  3. Add your Source Code  
  4. Using the provided Unit Tests
  5. Deliverables & Packaging

## Getting Started

The provided repo can be used as provided, simply make use of the following command to clone the repository.

```
git clone https://gitlab.com/adebarro_uom/cps1011-assignment-base-code.git
```

Once cloned, please do remember to remove the .git folder from the cloned project, we highly suggest that you make use of a personal Git repo to backup and store your code in a new GitLab repository of your choice.

Alternatively, you can also download the repository as a zip file.

## Repo Structure

The base code is structured as follows:

- *cps1011-basecode*
  - *includes*
    - minunit.h
    - question_1.h
    - question_2.h
  - CMakeLists.txt
  - main_1.c
  - main_2.c
  - question_1_unit_tests.c
  - question_2_unit_tests.c
  - question_1.c
  - question_2.c

### cps1011-basecode (Root Directory)

cps1011-basecode is the root folder. Any source files that you deem necessary for your code has to be provided under this directory. Failure to do so, will result in compilation errors since you will be uploading a zipped version of the cps1011-basecode (further details are provided in the Deliverables & Packaging section of the readme).

#### includes (Directory)

The includes directory contains all header files for the project. In case you need to add more header files, these should be added to this directory.

##### minunit.h

minunit.h is the header file containing the unit testing functionality. You must not alter this header file.

##### question_1.h & question_2.h

These header files specify the interfaces that you must implement in your assignment. Please note that you cannot tweak or change the provided interfaces in any way whatsoever. Details of the interfaces are provided in the assignment document.

#### CMakeLists.txt

```
cmake_minimum_required(VERSION 3.15)
project(CPS1011-Assignment)

set(CMAKE_C_STANDARD 11)
include_directories(includes)

# Add sources for the questions in the following variables
# IMP: Do not add the source file containing the main in these source variables
set(SOURCES_QUESTION_1 question_1.c)
set(SOURCES_QUESTION_2 question_2.c)

#IMP: See the repository for details about this CMakeLists structure
set(SOURCES_QUESTION_1_TESTS question_1_unit_tests.c ${SOURCES_QUESTION_1})
set(SOURCES_QUESTION_2_TESTS question_2_unit_tests.c ${SOURCES_QUESTION_2})

add_executable(Question1Executable main_1.c ${SOURCES_QUESTION_1})
add_executable(Question2Executable main_2.c ${SOURCES_QUESTION_2})

add_executable(Question1Tests ${SOURCES_QUESTION_1_TESTS})
add_executable(Question2Tests ${SOURCES_QUESTION_2_TESTS})
```

You should be quite familiar with CMakeLists by now, but for all intents and purposes you should only be modifying the following lines of code to add other source files:

```
set(SOURCES_QUESTION_1 question_1.c)
set(SOURCES_QUESTION_2 question_2.c)
```

It goes without saying, source files related to Question 1 should be added to SOURCES_QUESTION_1, and similarly, any source files pertaining to question 2 should be added to the SOURCES_QUESTION_2. 

We have already taken the liberty of adding an empty source file for both questions. **Important: No source files containing a main function should be added to these two variables (SOURCES_QUESTION_1 and SOURCES_QUESTION_2).**

#### main_1.c & main_2.c

These two source files provide an entry point for the two programs.

#### question_1_unit_tests.c & question_2_unit_tests.c

These source files contain some basic unit testing functionality for all functions to be implemented. *You must not alter these two source files. They are provided to guide you and help you make sure that the implementation at a minimum passes these tests (further details on running the unit tests are provided in the Using the Provided Unit Tests section)*.

#### question_1.c & question_2.c

These are empty source files. You should implement the provided interfaces here. Please note that you are allowed to add more source and header files to modularise your implementations further.

### Adding Source Code

As explained in the previous section, the implementation for the provided interfaces should be added to question_1.c and question_2.c. However, if you do find yourself requiring the use of further header/source files, this can be done as follows:

```
set(SOURCES_QUESTION_1 question_1.c foo_1.c)
set(SOURCES_QUESTION_2 question_2.c foo_2.c)
```

In the example above, foo_1.c is a new source file required for question_1.c and foo_2.c is another source file required for question_2.c. **Please note that at no point can any source file containing a main function be added to these two variables. A main entry point for each question has been provided (main_1.c main_2.c).**

## Using the provided Unit Tests

### What is a Unit Test

A unit test is a piece of code that checks if a particular function works as expected (further information on unit testing [here](https://en.wikipedia.org/wiki/Unit_testing#:~:text=Unit%20tests%20are%20typically%20automated,an%20individual%20function%20or%20procedure.)). For example (code taken for question_1_unit_tests.c):

```
MU_CUSTOM_TEST_START(testCleanAndValidate)
    int count = strlen(corpus);
    bool success = clean_and_validate(corpus);

    mu_custom_check(success,  1, 2);
    mu_custom_check(corpus[count - 1] == '\0',  2, 2);
MU_CUSTOM_TEST_END
```

This test checks that the *clean_and_validate* function manages to execute successfully for the provided text (specified by corpus). First, the unit test checks that the returned result is TRUE. Secondly, since the *clean_and_validate* function is expected to switch any \n to \0 at the end of the string, we also check that this is the case. If any of these checks fail, the user of the tests will be alerted, allowing he/she to update their implementation accordingly.

### Using the Tests

Out of the box the test executables (Question1Tests & Question2Tests) will fail to compile, since there is no implementation for the functions in the provided interfaces. You should start by adding dummy functions for the test executables to work.

Once that is done, you can execute the Question1Tests or Question2Tests executables (run the executables in CLion). Before there is a valid implementation, all tests will fail. However, as you complete more of the assignment, more tests should execute successfully. *Please note that the provided tests are not meant to exhaustively test your code. It is up to you to make sure that your code executes beyond the scope of the provided tests.*

## Deliverables and Packaging

This section will highlight how the source code has to be packaged and uploaded to VLE. Further details are given in the assignment document.

As previously explained, all source code required for your implementation must reside under the cps1011-basecode directory. Any code not present in this directory will not be taken into consideration. Once you feel that you have answered all the assignment questions, you should compress the cps1011-basecode directory as a ZIP archive (opening the zip file should show the cps1011-basecode directory and not any code that resides in the directory). **The use of any other compression format is prohibited.**

Your archive name must follow the format specified below:

**\<NAME\>_\<SURNAME\>_\<ID\>.zip**

e.g: joe_smith_112345L.zip

## Further Queries

Further queries should be posted to the unit's forum.  
