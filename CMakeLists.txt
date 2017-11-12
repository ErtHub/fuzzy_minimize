cmake_minimum_required(VERSION 3.7)
project(fuzzy_minimize)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp OperationImpl.h FuzzyFunction.cpp FuzzyFunction.h ImplicTable.cpp ImplicTable.h MinimizeAlgorithms.cpp MinimizeAlgorithms.h ImplicRow.cpp ImplicRow.h)
add_executable(fuzzy_minimize ${SOURCE_FILES})