# mpq_extract_all

Depends on [StormLib](https://github.com/ladislav-zezula/StormLib). Tested specifically against `v9.20` label.

After building StormLib, don't forget to `make install`.

If it's installed to a non-conventional path, set `STORM_ROOT` environment variable to the root of the install (directory that has `include/` and `lib/`) before running CMake.

## Building

```
mkdir build
cd build
cmake ..
make
```
