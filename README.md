Obtain Boost unit_test_framework for your platform to be discoverable by CMake. This is required for unit tests to work. This is currently the only external dependency.

The project builds test suites into `build/tests`. To run tests use thus `ctest --test-dir build/tests`. To make VS Code "CMake Test Explorer" discover the tests, using the `build` directory as CWD, use the following in your `.vscode/settings.json`.

```
{
    "cmakeExplorer.extraCtestLoadArgs": "--test-dir tests",
    "cmakeExplorer.extraCtestRunArgs": "--test-dir tests"
}
```