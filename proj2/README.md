# CMSC 341 - Project 2: Search & Rescue Swarm - Fall 2024

## Due: Tuesday Oct 22, before 9:00 pm

## Objectives
- Implementing a balanced binary search tree (BST) data structure.
- Practice writing rebalancing routines for an AVL tree.
- Practice writing re-structuring routines for a Splay tree.
- Practice use of recursion in programs.
- Practice writing unit tests.

## Introduction
Search and rescue (SAR) operations are an essential part of responding to disasters. SAR teams face many challenges in the field. Training the SAR teams is a very expensive and time-consuming process due to the different natures of disasters. Moreover, the SAR team members are in danger themselves during the operations. Fukushima nuclear accident in 2011 was a devastating disaster that required a search and rescue operation in a very dangerous environment.

With the recent advances in technologies such as robotics and Artificial Intelligence, many researchers are trying to replace the human team members with robots. The following figure presents an urban search and rescue robot moving across a rubble pile in an exercise by the National Institute of Standards and Technology (NIST).

*Figure from: National Institute of Standards and Technology*

A research team is trying to simulate search and rescue operations using a swarm of autonomous robotic devices. The robots communicate with each other and work as a team. The brain of every robot needs to be aware of the status of the whole team. You are assigned the task of developing a data structure that can store a database of all robots during the operation. The data structure runs in the brain of every robot, and it is updated automatically. The decision is made to use a self-balancing Binary Search Tree. The data structure module provides the possibility of organizing data in a regular BST or an AVL tree or a Splay tree. It is also possible to convert the data structure at run time.

## Binary Search Tree (BST)
A binary tree is a tree structure in which each node has either 0, 1, or 2 children. A BST is a derivative of a binary tree where each node contains a key-value pair. The key determines the nodes' placement in the tree, and the value is the data to be stored. Given a set of rules about how to order the keys, we can create a structure where we can query data from it with a specified key. For a BST, we define these rules as follows:

- If the target key is less than the key at the current node, traverse to the left child.
- If the target key is greater than the key at the current node, traverse to the right child.
- If the keys are equal, the action is determined by our application of the tree.

A BST on its own can be efficient, but as the dataset increases in size, we can start running into problems. In the worst case, our BST can become a linked list where each of the new keys is greater than or less than the previous one inserted. On the contrary, the best case is inserting elements into the tree in a way to make it a complete tree. Either case is rare to occur with a large dataset, but imbalances are common. An imbalance can be defined when one subtree on a node becomes significantly larger in size or height compared to the other subtree. As the tree becomes increasingly imbalanced, our average query times begin to increase. Luckily, we have methods to prevent large imbalances.

## The AVL Tree
An AVL tree employs rotations during insertions or deletions to balance a BST. As the name implies, nodes are literally rotated up the tree to keep its structure complete. A complete tree, or ideally a perfect tree, is the most efficient kind of binary tree. Insertions, deletions, and queries all take O(log(n)) time in such a case. AVL trees have two types of rotations, left and right, which are shown in the diagram below:

The variables "x" and "y" refer to 2 specific nodes whereas the subtrees "a", "b", and "c" refer to subtrees (which is just a pointer to a node which may or may not have more children). Note that the pointers to "a", "b", and/or "c" can be null, but "x" nor "y" will never be null.

The key to keeping an AVL tree efficient is when we perform these rotations. A rotation is performed on a node that is imbalanced, and an imbalance occurs when the node's children's heights differ by more than 1. For example, in the above diagram, consider node "y" to be imbalanced in the right rotation and node "x" to be imbalanced in the left rotation. Using a left and right rotation, we can perform four rotation combinations. The imbalance in the following examples occurs on the node with the height of 2 (in red).

- **Single left rotation**: This is a simple case where we can apply a left rotation to the top node to balance the tree.
- **Single right rotation**: Similar to the above case, we can apply a single right rotation to the top node to balance the tree.
- **Double left-right rotation**: This case requires two rotations. The first left rotation transforms the tree into a form we can balance with a second right rotation.
- **Double right-left rotation**: Likewise, this case uses a right rotation followed by a left rotation.

## The Splay Tree
Splay trees are binary search trees in which we store the recently accessed node at the root of the tree. Such a tree would be a good choice for a data structure if, in the application, some data points are accessed more frequently than others. Although some work is required in this data structure to bring up the recently accessed data point to the root, once a node is at the root, its next access time is O(1). The amortized analysis of Splay trees reveals that the search operation is O(log n).

## Assignment
Your assignment is to implement a binary search tree with balancing methods.

For this project, you are provided with the skeleton `.h` and `.cpp` files and a sample driver:

- **swarm.h** – Interface for the Robot and Swarm classes.
- **swarm.cpp** – A skeleton for the implementation of the class Swarm.
- **driver.cpp** – A sample driver program.
  - *Note: This file is provided to show a typical usage. Since the project is not implemented, trying to compile and run this driver program will not generate the sample output in `driver.txt`. Once you develop your project, you should be able to generate a similar output as `driver.txt` by running this driver program. Please note, the randomly generated values are different on different platforms.*
- **driver.txt** – A sample output produced by `driver.cpp`.

You may not change any of the private variables or public function declarations or file names. They will be used for grading. Also, any provided function implementations may not be modified. However, you may add your own private functions as helpers. The current private function declarations are provided as a backbone to help you.

Additionally, you are responsible for thoroughly testing your program. Your test program, `mytest.cpp`, must be submitted along with the other files. For grading purposes, your implementation will be tested on input data of varying sizes, including very large data. Your submission will also be checked for memory leaks and memory errors.

## Specifications
This project has three classes: **Random**, **Robot**, and **Swarm**. The Random class is provided as a supplementary class to facilitate testing. The Robot class defines the nodes in the binary tree. The Swarm class implements the balanced binary search tree. This class allows for creating three different types of BST. The three types are:

1. A regular BST with no balancing,
2. An AVL tree, and
3. A Splay tree.

### Class Swarm
The `Swarm` class implements a binary search tree. The `Swarm::m_root` member variable points to a node of type `Robot`. Every node in Swarm is a `Robot` object. The nodes are organized as a binary search tree. The `Swarm` class supports the insertion and deletion operations. After insertion or deletion operations, the class checks for any required properties, such as imbalance, and performs the re-structuring operations.

For the Swarm class, you must implement the following methods in `swarm.cpp`:

- **Swarm::Swarm()**: The default constructor performs the required initializations. It creates an empty object and sets the type of the tree to NONE.
- **Swarm::Swarm(TREETYPE type)**: This constructor creates an empty object and specifies the type of the tree (BST, AVL, or Splay).
- **Swarm::~Swarm()**: The destructor performs the required cleanup, including memory deallocations and re-initializing.
- **void Swarm::insert(const Robot& robot)**: This function inserts a `Robot` object into the tree using `Robot::m_id` as the key to traverse the tree.
- **void Swarm::clear()**: Deallocates all memory in the tree and makes it an empty tree.
- **void Swarm::remove(int id)**: Removes the node with the given `id` from the tree.
- **void Swarm::setType(TREETYPE type)**: Sets the type of an existing `Swarm` object and restructures the tree if necessary.
- **TREETYPE Swarm::getType() const**: Returns the current type of the tree.
- **const Swarm & Swarm::operator=(const Swarm & rhs)**: Overloads the assignment operator to create a deep copy of `rhs`.

### Additional Requirements
- You may not modify the class declarations `Swarm`, `Robot`, `Random`, or   any provided function implementations in `swarm.cpp`.
- No additional libraries may be used. However, private helper functions are permitted in the `Swarm` class.
- No STL containers or additional libraries may be used in the implementation of the `Swarm` class. However, you can use STL containers in the `Tester` class for testing purposes.
- The required functionality is provided in the `Robot` class, and no modifications to its implementation are necessary.
- Your code should not have any memory leaks or memory errors.
- The lowest level of nodes storing the keys should have a height of zero.
- Follow all coding standards as described in the C++ Coding Standards. Proper indentations and meaningful comments are essential.
  
The function **Swarm::dumpTree(...)** prints the node information in an in-order traversal. For every node, it prints the ID followed by the height of the node in the Swarm tree. The following example presents a sample output of the `dumpTree()` function for an AVL tree: ((((19224:0)19289:1(19372:0))20201:2((26241:0)27904:1))53002:3(60496:1(93209:0)))

The tree viewer tool can display a graphical representation of this output, facilitating debugging. You can copy and paste the dump output into the viewer.

## Testing
You need to test your project thoroughly, and you are required to submit your tests along with your project. Tests must be submitted in a file called `mytest.cpp`. The file name must be in lower case, as names like `myTest.cpp` are unacceptable.

The `mytest.cpp` file should contain the declaration and implementation of your `Tester` class and the `main()` function, along with all your test cases. For each test, the function should return true/false depending on whether the test passes or fails. Visual outputs are not accepted as test results. The test file must compile and run to completion without any interactive input.

An example of declaring, implementing, and calling a test function, as well as outputting the test results, was provided in the `driver.cpp` file for Project 0. Follow similar guidelines to structure your tests.

### Testing Guidelines
Testing incrementally makes finding bugs easier. Once a function is testable, ensure it works correctly before moving to the next function.

#### Testing the Swarm Class
1. Test whether the AVL tree is balanced after a decent number of insertions, e.g., 300 insertions.
   - This requires visiting all nodes and verifying that the height values are correct.
2. Test whether the BST property is preserved after all insertions.
   - This requires visiting all nodes and comparing key values.
3. Test whether the Splay tree performs the splay operations.
   - After inserting multiple nodes into the Splay tree, check whether the inserted node becomes the root and if the tree maintains the BST property.
4. Test the height values after multiple insertions in a Splay tree.
5. Test the remove function for a normal case in the BST tree.
   - Removing a node from the tree should result in the tree without the node.
6. Test the remove function for an edge case in the BST tree.
   - If the tree has only one node, removing that node should leave the tree empty.
7. Test whether the AVL tree is balanced after multiple removals.
   - For example, insert 300 nodes, remove 150 nodes, and check if the AVL property is preserved.
8. Test whether the BST property is preserved after multiple removals from both BST and AVL trees.
9. Test whether the height values are correct in a BST tree after multiple removals.
10. Test the assignment operator for a normal case.
11. Test the assignment operator for an error case, e.g., assigning an empty object to another empty object.

### Random Numbers for Testing
For testing purposes, you can use fixed data values or generate random data. You can generate random data using the provided `Random` class. The class uses a default seed value and generates pseudorandom numbers, which means the numbers are not truly random but will be consistent on the same machine. However, note that the output may differ between different platforms.

## Memory Leaks and Errors
Run your test program in `valgrind` to check for memory leaks and errors. If valgrind finds memory errors, compile your code with the `-g` option to enable debugging support and then rerun valgrind with the `-s` and `--track-origins=yes` options. Valgrind will show you the line numbers where the errors are detected and can usually identify the cause of the error.

**Important**: Never ignore compiler warnings. They are often indicators of potential errors in your program.

## What to Submit
You must submit the following files to the `proj2` directory:

- `swarm.h`
- `swarm.cpp`
- `mytest.cpp` (This file must contain the declaration and implementation of your `Tester` class, all test cases, and the `main()` function.)

If you have followed the instructions on the Project Submission page to set up your directories, you can submit your code using the following command:

```bash
cp swarm.h swarm.cpp mytest.cpp ~/cs341proj/proj2/
```
### Grading Rubric
The following presents a course rubric. It shows how a submitted project might lose points.

Conforming to coding standards make about 10% of the grade.
Your test program is worth 50%. If you submit the sample driver program as your test program or no test program is submitted there will be 50% deduction.
Correctness and completeness of your test cases (mytest.cpp) make about 15% of the grade.
We have written test cases to test your submission without knowing anything about your code. Therefore, it is extremely important that your submission conforms to the specified requirements. Passing tests make about 30% of the grade.
There is a 5% deduction for every modification that we need to perform to compile and run your work. For example, if we need to rename your file from myTest.cpp to mytest.cpp the deduction will be applied.
If the submitted project is in a state that receives the deduction for all above items, it will be graded for efforts. The grade will depend on the required efforts to complete such a work.




