# diabutil

Stand-alone utilities for decoding Diablo files. Written in C++.

Each directory has it's own README.

## Setup

1. Install git, cmake, c++17 compiler, stormlib dependencies (zlib, bzib2)
1. Clone submodules to get StormLib.

## Building

```
mkdir build
cd build
cmake ..
make
```

Works on Windows using Visual Studio Code with the CMake Tools extension.

## Running Unit Tests

This runs tests integrated with CMake, which is currently just gtest C++ tests

1. Build
2. `ctest`

## Running Integration Tests

This runs the produced binary using golden files, etc. Some of these might be candidates for gtest tests in the future.

Setup:

1. pip install --user pipenv
1. cd integration_tests
1. pipenv install
1. pipenv run python run_integration_test.py

## Contributing

1. Tests must pass (both unit and integration tests)
2. Must run `clang-format`