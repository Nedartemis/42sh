cmake_minimum_required(VERSION 3.22)
project(42sh VERSION 1.0.0 LANGUAGES C)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_library(common_options INTERFACE)
target_compile_options(common_options INTERFACE
    "-Wall"
    "-Wextra"
    "-Wvla"
    "-pedantic"
    "-Werror"
    "-std=c99"
    "-D_XOPEN_SOURCE=700"
    "-fsanitize=address"
    "-g"
)

add_subdirectory("src")

option (ENABLE_TESTS "Build tests along with the library" OFF)
if (ENABLE_TESTS)
    add_subdirectory("tests/criterion_test")
endif ()

# dependencies order
# sh
# execution
# simple_command
# builtin
# parser
# AST
# lexer
# tools
# pipe
