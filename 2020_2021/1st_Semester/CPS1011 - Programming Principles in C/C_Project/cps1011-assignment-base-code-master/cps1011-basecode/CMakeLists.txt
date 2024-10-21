cmake_minimum_required(VERSION 3.15)
project(CPS1011-Assignment)
 
set(CMAKE_C_STANDARD 11)
include_directories(includes)

# Add sources for the questions in the following variables
# IMP: Do not add the source file containing the main in these source variables
set(SOURCES_QUESTION_1 question_1.c)
set(SOURCES_QUESTION_2 question_2.c)
add_library(libraryq1 SHARED ${SOURCES_QUESTION_1})
add_library(libraryq2 SHARED ${SOURCES_QUESTION_2})

#IMP: See the repository for details about this CMakeLists structure
set(SOURCES_QUESTION_1_TESTS question_1_unit_tests.c ${SOURCES_QUESTION_1})
set(SOURCES_QUESTION_2_TESTS question_2_unit_tests.c ${SOURCES_QUESTION_2})
 
add_executable(Question1Executable main_1.c ${SOURCES_QUESTION_1})
add_executable(Question2Executable main_2.c ${SOURCES_QUESTION_2})
target_link_libraries(Question2Executable libraryq2 libraryq1)

add_executable(Question1Tests ${SOURCES_QUESTION_1_TESTS})
add_executable(Question2Tests ${SOURCES_QUESTION_2_TESTS})
