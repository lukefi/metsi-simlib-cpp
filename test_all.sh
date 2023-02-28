#!/bin/bash
set -e
cmake . -B test_build
cmake --build test_build
ctest --test-dir test_build/tests
rm -rf test_build
