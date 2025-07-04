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
   make gp_simulator
   make gp_trainer
   ```

3. **Run the Program**
   Execute the compiled binaries with the appropriate library path:

   ```bash
   LD_LIBRARY_PATH=~/opt/c/sdl/lib ./build/gp_simulator [input.bin]
   ```

* `gp_simulator`: The `input.bin` file is optional.

   ```bash
   LD_LIBRARY_PATH=~/opt/c/sdl/lib ./build/gp_trainer -g <generations-count> -o <output.bin>
   ```

* `gp_trainer`:

  * `<generations-count>` specifies the number of generations to run.
  * `<output.bin>` is the output file generated after the given number of generations.

- **`gp_trainer` Options:**

   ```
   -g <NUMBER>     Number of generations to run (default: 69)
   -o <FILEPATH>   Output file path (default: output.bin)
   -gp             Enable epic Gnuplot reporting (requires Gnuplot)
   -h              Display this help message
   ```

4. **Clean Up**
   To remove the compiled executable and other build files:

   ```bash
   make clean
   ```
### Demo
[gp_simulator](https://github.com/user-attachments/assets/3e6c6395-3e49-4054-9898-22cc2342ac3e)


## Controls

### Keyboard

| Key              | Action                       |
| ---------------- | ---------------------------- |
| <kbd>R</kbd>     | Generate a new game state    |
| <kbd>Space</kbd> | Advance the game by one step |

### Mouse

* Click on agents (red triangles) to print their state to stdout.


## References

* [Genetic Algorithm – Wikipedia](https://en.wikipedia.org/wiki/Genetic_algorithm)
* [SDL – Official Website](https://www.libsdl.org/)
