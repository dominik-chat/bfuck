# bfuck

bfuck is a simple brainfuck interpreter.

## Compilation

The program requires a C compiler and [cmake](https://cmake.org/).

The compilation compilation of the program can be performed in the root directory:
```
cmake -S. -Bbuild
cd build
make
```

The program can be installed using this command:
```
make install
```
**Note that you may be required to execute this command as a superuser**
