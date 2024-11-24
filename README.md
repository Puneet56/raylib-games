# Breakout

Collection of small games made using [Raylib](https://github.com/raysan5/raylib)
An attempt to learn game programming.

## Instructions to run
- Make sure raylib is installed on your system. ([Instructions](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation)). The project uses `pkg-config` in Makefile. Make sure the commands below output correct paths
  ```sh
  pkg-config --cflags raylib
  pkg-config --libs raylib
  ```

- Create a [compile_commands.json](https://clang.llvm.org/docs/JSONCompilationDatabase.html) file for LSP setup using [this](https://texttoolkit.com/compilation-database-generator) or any other tool.

- Build and run the project
  ```sh
  make run PROJECT=<project-name>
  ```

- All projects are located in `src` directory and project name is filename without extension.

## Projects list
- breakout 



