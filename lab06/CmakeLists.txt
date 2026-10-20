cmake_minimum_required(VERSION 3.16)
project(CSCI41_Lab06 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(lab06
    src/main.cpp
    src/int_set.cpp
)

add_executable(lab06_tests
    tests/test_lab06.cpp
    src/int_set.cpp
)

enable_testing()
add_test(NAME lab06_public COMMAND lab06_tests)
