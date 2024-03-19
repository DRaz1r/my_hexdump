# Build

## C++20

**Attention:** CMake will clone repository with git.

```
mkdir build
cd build/
cmake ..
make
```

## C++11

```
mkdir build
cd build/
cmake ..
make
```

# Run
```
./hexdump -h
A command line utility for printing the hexadecimal and ASCII representation of a file.
Usage:
  hexdump [OPTION...]

  -f, --file arg   Input file (default: -)
  -w, --width arg  Number of bytes per line (default: 16)
  -h, --help       Print usage
```