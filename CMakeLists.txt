cmake_minimum_required(VERSION 3.7)
project(fuzzy_minimize)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp FuzzyFunction.cpp FuzzyFunction.h CubeTable.cpp CubeTable.h CubeRow.cpp CubeRow.h Parser.cpp Parser.h Scanner.cpp Scanner.h Reader.cpp Reader.h Minimizer.cpp Minimizer.h FunctionGenerator.cpp FunctionGenerator.h)
add_executable(fuzzy_minimize ${SOURCE_FILES})

add_definitions(-DSORT_TABLES)
add_definitions(-DCONDENSE_CUBEROWS)