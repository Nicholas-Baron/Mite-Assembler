#!/bin/bash

# Do the CMake -> Make build
mkdir -p build
cd build || exit 1
cmake .. || exit 1
make  || exit 1
cd .. || exit 1

# copy mite to here
cp build/mite .

# run all tests
for file in tests/*.txt; do
	time ./mite $file -v
done
