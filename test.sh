#!/bin/bash

# Do the CMake -> Make build
mkdir -p build
cd build || exit 1
cmake ..
make
cd .. || exit 1

# copy mite to here
cp build/mite .

# run all tests
for file in tests/*; do
	./mite $file
done
