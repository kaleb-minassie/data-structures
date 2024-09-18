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
- Testing Snakes Class
- Test the alternative constructor for a normal case. It must create a default board.
- Test the alternative constructor for an error case. That would be a board size which is not a square value.
- Test the function reStart() and check whether it reinitializes the game.
- Test the function play(...) for a valid move on a ladder.
- Test the function play(...) for a valid move on a snake.
- Test the function makeDefaultBoard(...) for a normal case.
- Test the function makeRandomBoard(...) for a normal case.
- Test the function makeRandomBoard(...) for an error case.
- Test the overloaded assignment operator for a normal case.
- Test the overloaded assignment operator for an edge case.

### Testing For Memory Leaks / Memory Errors
Run your test program in valgrind; check that there are no memory leaks or errors.
Note: If valgrind finds memory errors, compile your code with the -g option to enable debugging support and then re-run valgrind with the -s and --track-origins=yes options. valgrind will show you the line numbers where the errors are detected and can usually tell you which line is causing the error.
Never ignore warnings. They are a major source of errors in a program.

### What to Submit
You must submit the following files to the `proj1` submit directory:

- `snakes.h`
- `snakes.cpp`
- `mytest.cpp` (Note: This file contains the declaration and implementation of your `Tester` class as well as all your test cases.)

If you followed the instructions in the Project Submission page to set up your directories, you can submit your code using the following command:

```bash
cp snakes.h snakes.cpp mytest.cpp ~/cs341proj/proj1/
```
### Grading Rubric
The following presents a course rubric. It shows how a submitted project might lose points.

Conforming to coding standards make about 10% of the grade.
Your test program is worth 50%. If you submit the sample driver program as your test program or no test program is submitted there will be 50% deduction.
Correctness and completeness of your test cases (mytest.cpp) make about 15% of the grade.
We have written test cases to test your submission without knowing anything about your code. Therefore, it is extremely important that your submission conforms to the specified requirements. Passing tests make about 30% of the grade.
There is a 5% deduction for every modification that we need to perform to compile and run your work. For example, if we need to rename your file from myTest.cpp to mytest.cpp the deduction will be applied.
If the submitted project is in a state that receives the deduction for all above items, it will be graded for efforts. The grade will depend on the required efforts to complete such a work.


Good luck, and happy coding!

