# Metsi simulation library as C++

Recreation of https://github.com/lukefi/metsi simulation core as a C++ library.

## Dependencies

### yaml-cpp

Housed as a git submodule in `deps/yaml-cpp`.
Initialize with the command `git submodule update --init deps/yaml-cpp`.

### csv-parser

Housed as a git submodule in `deps/csv-parser`.
Initialize with the command `git submodule update --init deps/csv-parser`.

### LuaJIT-2.1

Obtain separately to be discoverable by CMake.

### Boost-1.74.0 (unit_test_framework)

Obtain separately to be discoverable by CMake.

## Notes

You can use the `test_all.sh` to do a single full build and test suites run.
