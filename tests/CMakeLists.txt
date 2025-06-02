cmake_minimum_required(VERSION 3.30)
project(unit-test VERSION 1.0 LANGUAGES C)
add_compile_options(-Wall -Wextra -Werror -Wconversion -Wunused-result)
add_executable(
	unit-test
	unit-test.c
	../src/c-string.c
)
target_include_directories(unit-test PUBLIC ../include)
