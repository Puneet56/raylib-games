# Breakout

Breakout game made using [Raylib](https://github.com/raysan5/raylib)

## Instructions to run
- Make sure raylib is installed on your system. ([Instructions](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation)). The project uses `pkg-config` in Makefile. Make sure the commands below output correct paths
```sh
pkg-config --cflags raylib
pkg-config --libs raylib
```

- Create a [compile_commands.json](https://clang.llvm.org/docs/JSONCompilationDatabase.html) file for LSP setup using [this](https://texttoolkit.com/compilation-database-generator) or any other tool.

- make
```sh
make run
```


