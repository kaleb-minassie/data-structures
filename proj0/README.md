# CMSC341 - Project 0: Peg Solitaire - Fall 2024

## Project Overview

In this project, you will develop the game engine for **Peg Solitaire**, a one-player board game with the objective of leaving only one peg on the board by jumping over adjacent pegs. The board supports different shapes, specifically the **English** and **Diamond** boards.

### Due Date
- **Due Date:** Tuesday, September 10, before 9:00 pm

### Objectives
1. **C++ Memory Management**: Practice allocating and deallocating memory dynamically.
2. **Valgrind**: Use Valgrind to check for memory leaks.
3. **Test Cases**: Write comprehensive test cases to verify your program.
4. **Requirements Analysis**: Practice analyzing and clarifying a project's requirements.

### Game Rules
- The game begins with a fully populated board except for the central hole.
- A valid move is to jump a peg orthogonally over an adjacent peg into a hole two spaces away, removing the jumped peg.
- The goal is to empty the board except for one peg in the center.

### Board Shapes
- **English Board**: 33 holes, 32 marbles.
- **Diamond Board**: 41 holes, 40 marbles.

### Assignment Steps
1. **Create a Working Directory**: Set up your project workspace.
2. **Implement the `Solitaire` Class**: Complete the implementation of the `Solitaire` class, which stores the board and manages gameplay logic.
3. **Testing**: Write a comprehensive test file `mytest.cpp` that thoroughly tests normal cases, edge cases, and error cases.
4. **Memory Leak Checking**: Use Valgrind to ensure there are no memory leaks in your program.

### Specifications
- **Data Structure**: The `Solitaire` class uses a 2D array to represent the board. Each cell stores an integer value representing either a marble, hole, or an out-of-bounds area.
- **Memory Management**: Ensure that all dynamic memory is correctly allocated and deallocated.
- **Game Play Function**: Implement the logic to handle valid moves and update the board accordingly.

### Class Overview

#### `Solitaire`
- **Constructor**: Initializes the board with the given shape.
- **Destructor**: Properly deallocates memory.
- **play(origin, destination)**: Moves a peg from the origin to the destination if the move is valid.
- **newBoard()**: Resets the board for a new game.
- **changeBoard()**: Switches the board to a different shape.
- **copy constructor**: Creates a deep copy of a `Solitaire` object.

### Testing
You are required to write test cases in a file called `mytest.cpp` to ensure your program works correctly. Test for:
- Normal cases (valid moves, correct board initialization)
- Edge cases (invalid moves, empty board)
- Error cases (unsupported board shapes, copy constructor errors)

### Valgrind
Run your program through Valgrind to check for memory leaks. A successful run will show no memory in use at exit and no errors.

### What to Submit
You must submit the following files:
- `solitaire.h`
- `solitaire.cpp`
- `mytest.cpp`

Submit your files to the appropriate project directory using the `cp` command:

