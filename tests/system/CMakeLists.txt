cmake_minimum_required(VERSION 3.30.1)
project(SYSTEM_TEST)
add_compile_options(-Wall -Wextra -Werror -Wconversion -Wunused-result)
# add_compile_options(-Wall -Wextra -Wconversion -Wunused-result)
add_executable(
	SYSTEM_TEST
	system_test.c
	../../src/string.c
)
