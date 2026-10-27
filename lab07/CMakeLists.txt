cmake_minimum_required(VERSION 3.16)
project(CSCI41_Lab07 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# The demo program.
add_executable(lab07
    src/main.cpp
    src/playlist.cpp
)

# The public self-check. Your grade comes from a larger private set that is
# compiled the same way, against your src/playlist.cpp.
add_executable(lab07_tests
    tests/test_lab07.cpp
    src/playlist.cpp
)

enable_testing()
add_test(NAME lab07_public COMMAND lab07_tests)
