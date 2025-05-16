#!/bin/env sh

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on &&
make &&
./UNIT_TESTS
echo "Program exited with status code:" $?
