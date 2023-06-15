# Building

There are two sides of the code base:

- host: These are utilities that run on the build machine and are used to produce the final product.
- win32: This is code designed to be distributed as part of the final package and can only be compiled for 32-bit Windows (either via MSVC or mingw)

## Building Host

Requires CMake and a C++17 compiler

```
mkdir build-host
cd build-host
cmake ..
cmake --build .
```

## Building Win32 (MSVC)

Requires CMake and a C++20 compiler

```
mkdir build-win32
cd build-win32
cmake .. -DPREABLO_TARGET=win32 -G "Visual Studio 17 2022" -A Win32
cmake --build .
```

## Building Win32 (mingw)

Requires CMake and a C++20 compiler

```
mkdir build-win32
cd build-win32
cmake .. -DPREABLO_TARGET=win32 -DCMAKE_TOOLCHAIN_FILE=../mingw.toolchain.cmake
cmake --build .
```