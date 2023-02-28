# Metsi simulation library as C++

Recreation of https://github.com/lukefi/metsi simulation core as a C++ library.

## Dependencies

### yaml-cpp

Obtain separately to be discoverable by CMake.

### Boost unit_test_framework

Obtain separately to be discoverable by CMake.

### csv-parser

Housed as a git submodule in `deps/csv-parser`.
Initialize with the command `git submodule update --init deps/csv-parser`.

## Notes

The project builds test suites into `build/tests`. To run tests use thus `ctest --test-dir build/tests`. To make VS Code "CMake Test Explorer" discover the tests, using the `build` directory as CWD, use the following in your `.vscode/settings.json`.

```
{
    "cmakeExplorer.extraCtestLoadArgs": "--test-dir tests",
    "cmakeExplorer.extraCtestRunArgs": "--test-dir tests"
}
```

You can use the `test_all.sh` to do a single full build and test suites run.