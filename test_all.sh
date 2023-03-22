#!/bin/bash
set -e
cmake . -DCMAKE_TOOLCHAIN_FILE=./deps/vcpkg/scripts/buildsystems/vcpkg.cmake -B test_build
cmake --build test_build
ctest --test-dir test_build/metsi-app
ctest --test-dir test_build/metsi-simlib
rm -rf test_build
