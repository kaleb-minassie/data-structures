# CMSC341 - Project 1: Snakes and Ladders - Fall 2024

## Project Overview

In this project, you will develop a game engine for the popular board game **Snakes and Ladders**. The board is represented as a singly linked list, and players will traverse the list based on dice rolls. Players move across the board and interact with snakes (which move them down) and ladders (which move them up).

### Due Date
- **Due Date:** Tuesday, October 1, before 9:00 pm

### Objectives
1. **Linked List Implementation**: Practice implementing a linked list data structure.
2. **Dynamic Memory Management**: Manage memory efficiently using C++ (avoid memory leaks and access violations).
3. **Test Cases**: Write test cases to ensure your project works correctly under normal, edge, and error cases.
4. **Project Requirement Analysis**: Develop a deep understanding of the project's requirements to build a functional game engine.

### Game Rules
- The board is a square grid (e.g., 3x3 for a board size of 9).
- **Snakes** move players downward, and **Ladders** move players upward.
- Players start at cell 1.
- The board must contain snakes and ladders, and their total number should be about 1/4th of the total board size.
- The number of snakes and ladders must be balanced with a tolerance of 1.
- No snake or ladder can appear on the first or last cells of the board.

### Assignment

Your task is to implement the game engine using the `Snakes` class. The board is represented as a linked list, where each player traverses the board. You will implement the class and its methods, create a random or default board, and simulate the game.

### Provided Files
- `snakes.h`: Interface for the `Snakes` class.
- `snakes.cpp`: Skeleton code for the game engine.
- `driver.cpp`: A sample driver for testing basic game functionality.
- `driver.txt`: Sample output of `driver.cpp`.

### Specifications

#### `Snakes` Class
- **`Snakes::Snakes()`**: Default constructor that initializes an empty object.
- **`Snakes::~Snakes()`**: Destructor that properly deallocates all dynamic memory.
- **`Snakes::Snakes(int boardSize)`**: Constructor that creates a default board of the given size.
- **`bool Snakes::makeDefaultBoard(int boardSize)`**: Creates a default board and returns `true` if successful.
- **`void Snakes::makeRandomBoard(int boardSize, int numSnakesLadders)`**: Creates a random board with a specified number of snakes and ladders.
- **`bool Snakes::play(int dice)`**: Simulates a player's turn based on a dice roll. Returns `false` when the game ends (i.e., when a player reaches the last cell).
- **`int Snakes::rollDice()`**: Returns a random number between 1 and 6 to simulate a dice roll.
- **`void Snakes::reStart()`**: Resets the game without modifying the board.
- **`const Snakes & Snakes::operator=(const Snakes & rhs)`**: Deep copy assignment operator.
- **`void Snakes::dumpBoard()`**: Prints the current state of the board (for debugging).

### Additional Requirements
- **No Modifications**: The `Snakes` and `Cell` class declarations must not be modified. Only private helper functions can be added.
- **No STL Containers**: You may not use STL containers in the `Snakes` class but can use them in your test file.
- **Dynamic Memory Management**: Implement proper memory management to avoid memory leaks or invalid accesses.

### Testing
You are required to test your program thoroughly using a separate file named `mytest.cpp`, which contains your `Tester` class and all test cases. Each test case must be written in a separate function, and each function should return `true` or `false` based on whether the test passes.

#### Sample Tests
- Test the constructor for a default board.
- Test the behavior of the board when snakes or ladders are present.
- Test edge cases such as incorrect board sizes or invalid moves.
- Ensure that all dynamic memory is correctly deallocated, using **Valgrind** to detect memory leaks.

### Memory Leak Detection
Use **Valgrind** to check for memory leaks in your implementation:
```bash
valgrind ./your_test_program.out
Ensure there are no memory leaks or segmentation faults.

### What to Submit
You must submit the following files to the `proj1` submit directory:

- `snakes.h`
- `snakes.cpp`
- `mytest.cpp` (Note: This file contains the declaration and implementation of your `Tester` class as well as all your test cases.)

If you followed the instructions in the Project Submission page to set up your directories, you can submit your code using the following command:

```bash
cp snakes.h snakes.cpp mytest.cpp ~/cs341proj/proj1/

### Grading Rubric
- **Coding Standards (10%)**: Adhere to the course coding standards.
- **Test Cases (15%)**: Provide thorough test cases in `mytest.cpp`.
- **Correctness (75%)**: Your code will be tested against our test cases to verify functionality.
- **Commit History**: Regular commits are expected to reflect your progress throughout the project.

Good luck, and happy coding!

