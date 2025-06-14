# Genetic Programming

Genetic Programming (GP) is a type of evolutionary algorithm and a subset of machine learning. It automatically evolves computer programs to solve complex problems, drawing inspiration from the principles of natural selection and biological evolution.

## Getting Started

1. **Install SDL**
   Ensure that the `SDL.h` library is installed on your system. Follow the instructions provided in [DEV.md](./DEV.md).
   If you've installed SDL in a different location, update the `PREFIX` variable accordingly in the Makefile:

   ```makefile
   PREFIX = $(HOME)/opt/c/sdl
   ```

2. **Build the Program**
   Compile the project using:

   ```bash
   make gp
   ```

3. **Run the Program**
   Execute the compiled binary with the proper library path:

   ```bash
   LD_LIBRARY_PATH=~/opt/c/sdl/lib ./build/gp
   ```

4. **Clean Up**
   To remove the compiled executable and other build files:

   ```bash
   make clean
   ```

## References

* [Genetic Algorithm – Wikipedia](https://en.wikipedia.org/wiki/Genetic_algorithm)
* [SDL – Official Website](https://www.libsdl.org/)
