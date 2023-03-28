# Metsi simulation library as C++

Recreation of parts of  https://github.com/lukefi/metsi simulator as a C++ project.
We implement the simulate and export phases of the Python application as a native application for performance.
We aim to eventually establish feature parity between these projects.

This repository hosts a CMake based C++20 project with two separate components: `metsi-app` and `metsi-simlib`.
After main development, they should be split as separate projects.

`metsi-app` is an application for arranging simulation events and simulation state on the domain of forestry.
It utilizes YAML based configuration for simulation events and simulation alternatives tree.

`metsi-simlib` is a template collection for arranging and running an alternative paths simulation.
It also provides certain non-domain-specific utilities for arranging simulations, with the aim to be eventualy called a simulator framework.

## Working with the project

The following subsections describe the main concepts for working with this project.

### Dependencies

LuaJIT dependency is included as a submodule.
The csv-parser dependency is included as a single header source file in-project.
Other dependencies are managed by vcpkg as a submodule.
Initialize by running

```
git submodule update --init deps/luajit
git submodule update --init deps/vcpkg
./deps/vcpkg/bootstrap-vcpkg.bat # on Windows PowerShell
./deps/vcpkg/bootstrap-vcpkg.sh  # on Linux/OSX shells or Windows Git BASH shell
```

This initializes the luajit submodule dependency.
For vcpkg it essentially downloads `vcpkg.exe` on Windows or `vcpkg` on Linux or OSX, building it if necessary.
For Windows, you can also manually obtain vcpkg.exe from https://github.com/microsoft/vcpkg-tool/releases/download/2023-03-01/vcpkg.exe and insert it into the `deps/vcpkg` directory.

Resolving and installing dependency libraries should be performed automatically later by CMake configuration (see below).

Table of dependency libraries:

| dependency | version | source                                      | license | note                                                             |
|------------|---------|---------------------------------------------|---------|------------------------------------------------------------------|
| yaml-cpp   | 0.7.0   | https://github.com/jbeder/yaml-cpp          | MIT     ||
| csv-parser | 2.1.3   | https://github.com/vincentlaucsb/csv-parser | MIT     | Included as single header copy `src/metsi-app/src/csv_parser.hpp |
| boost      | 0.81.0  | https://github.com/boostorg/boost           | BSL-1.0 | boost-test, boost-program-options, boost-lexical-cast            |
| luajit     | 2.1     | https://github.com/WohlSoft/LuaJIT          | MIT     | A fork of actual luajit repo, adding CMake build possibility     |

### CMake Presets

The `CMakePresets.json` file specifies several presets for configuring environments and building.
All named presets work as both configuration presets and build presets.

* `ninja-debug` Debug target for Macintosh or Linux hosts using CLang or GCC toolchain and Ninja generator
* `ninja-release` Release target for Macintosh or Linux hosts using CLang or GCC toolchain and Ninja generator
* `msbuild-debug` Debug target for Windows hosts using the MSVC toolchain and MSBuild generator
* `msbuild-release` Release target for Windows hosts using the MSVC toolchain and MSBuild generator

All presets assume vcpkg availability in `deps/vcpkg`.

### Configure

Using Visual Studio, Visual Studio Code or CLion should automate this phase with CMake.
To run configure manually you can call `cmake . --preset preset-name` where preset name is a known configuration preset.
All presets generate build directories under `builds/preset-name`.

To run configure manually without presets, you need to pass the `CMAKE_TOOLCHAIN_FILE` parameter for CMake be able to resolve depedency libraries via vcpkg.
Call `cmake . -DCMAKE_TOOLCHAIN_FILE="$PWD/deps/vcpkg/scripts/buildsystems/vcpkg.cmake" output-dir` where output-dir is a directory path of your choosing.

Without the toolchain file, dependencies need to be discoverable from the host system standard locations via CMake.
You will need to know yourself what you're doing with this option.

### Build

Use your IDE's tools for building.

To build manually you can call `cmake --preset preset-name` where preset name is a known configuration preset.
Build results are made under `builds/preset-name`.

For the application, a `metsi` or `metsi.exe` is produced with a reference `forest_stand.csv` and `control.yaml` files alongside the binary.

### Tests

This project has builtin Boost tests in the two subprojects.
These tests are expanded during the CMake build process as test suites and test cases for CTest.
CTest is the test runner utility used alongside with CMake projects.
IDEs may use other methods for running the tests.

CLion is able to pick up and run tests from both components out-of-the-box on any platform.

There is not yet any known configuration for Visual Studio Code or Visual Studio to be able to pick up the tests.
At the moment it appears that the test explorers on those IDEs are unable to sense the tests from this project layout.
This is a work in progress to solve.

You can invoke the `test_all.sh` script to do a single full build and test suites run on a shell CLI.
This does not use CMake presets.
It should just work in a typical host environment with CMake and a C++ toolchain available.

### Run

Use your IDE's tools for running and debugging the application.

For CLI usage, look in the CMake build directory for build results.
Run currently takes 2 mandatory and 1 optional CLI arguments:

```
metsi <input-file-path> <output-directory-path> [<control-file>]
```

control-file defaults to `./control.yaml`.

A default example run would be 

```
./metsi forest_data.csv output-directory control.yaml
```

### Platform specific notes

On Windows, you need to obtain Visual Studio 2022 (at least Community Edition).
This will require Administrator privileges on your host.
Use the Visual Studio Installer to obtain the MSVC CLI tools along with the version 17 MSVC toolchain.
With this setup, the `msbuild-*` presets should work out of the box on any IDE or otherwise.
It should be possible to use MinGW or other configurations with the `ninja-*` presets, but this has not been explored.

