cmake_minimum_required(VERSION 3.30.1)
project(UNIT_TESTS)
add_compile_options(-Wall -Wextra -Werror -Wconversion -Wunused-result)
# add_compile_options(-Wall -Wextra -Wconversion -Wunused-result)
add_executable(
	UNIT_TESTS
	unit_tests.c
	../../src/string.c
)
