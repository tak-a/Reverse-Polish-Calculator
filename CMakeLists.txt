cmake_minimum_required(VERSION 3.8)
project(calcutor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(SOURCE_FILES main.cpp expression.h token.h program.h fonctions.h verification.h expression.cpp token.cpp program.cpp fonctions.cpp verification.cpp)
add_executable(calculator ${SOURCE_FILES})
