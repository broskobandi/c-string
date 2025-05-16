#!/bin/env sh

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on &&
make &&
./SYSTEM_TEST
echo "Program exited with status code:" $?
