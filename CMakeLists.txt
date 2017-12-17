cmake_minimum_required(VERSION 3.7)
project(fuzzy_minimize)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp OperationImpl.h FuzzyFunction.cpp FuzzyFunction.h CubeTable.cpp CubeTable.h CubeRow.cpp CubeRow.h Parser.cpp Parser.h Scan.cpp Scan.h Source.cpp Source.h Util.cpp Util.h Minimizer.cpp Minimizer.h)
add_executable(fuzzy_minimize ${SOURCE_FILES})