add_executable(unit-test unit-test.c ${PROJECT_SOURCE_DIR}/src/c-string.c)
target_include_directories(unit-test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(unit-test c-string)
