cmake_minimum_required(VERSION 3.30)
project(unit_test)
add_executable(
	unit_test
	../src/c-string.c
	../src/c-string-utils.c
	unit_test.c
)
