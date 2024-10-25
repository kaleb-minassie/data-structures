# CMSC 341 - Project 3: Manufacturing Scheduler - Fall 2024

## Due: Tuesday Nov 12, before 9:00 pm

## Objectives

- Practice constructing and using heap data structure as a priority queue ADT.
- Practice writing merge operation in skew and leftist heap data structures.
- Gain additional experience constructing and using binary trees.
- Practice using recursion in programs.
- Learn to use function pointers.
- Practice writing unit tests.
- Practice working in a Linux environment.
- Practice analyzing and understanding a project requirements.

## Introduction

In today’s world, different factors such as worldwide health pandemics and geopolitical changes cause many disruptions in the world. Due to these problems, manufacturing plants are late on fulfilling their orders. There is a long list of products that are affected; among them we can name electronic chips, cars, processed food, etc. The worldwide problems prevent manufacturers from receiving the raw materials on time. To mitigate this, manufacturing plants need to prioritize their production.

You are assigned a task to implement a scheduler software application that can be used by production managers. The application automatically prioritizes the production of orders. The prioritization is based on some specific criteria. The algorithm that uses the criteria to calculate the priorities can change. Therefore, the application should have the possibility of changing the order priorities. An algorithm that can determine the priority will be implemented in a function. Your job is to implement a priority queue data structure that takes different priority functions. Such an architecture allows us to use different priority functions.

## Heap

### Skew Heap

A skew heap is a specialized version of a heap data structure which performs the insertion and deletion operations in O(log n) amortized time. This data structure is a binary tree in which the root always holds the node with the highest priority. Skew heap uses merge operation to perform insertion and deletion.

The major operations supported by a skew heap are insertion of elements, reading the highest priority element, and removing the highest priority element. Reading the highest priority element is just a matter of reading the root node of the heap. The other two operations, insertion and removal, are applications of the merge function:

- To insert a new node `x` into an existing skew heap `H`, we treat `x` as a single-node skew heap and merge it with `H`.
- To remove the node with the highest priority value, we delete the root node and then merge the root's left and right sub-heaps.

We see, then, that the merge function is key to all of the major skew heap operations. If we can implement merge correctly, insertion and removal are simple.

The special feature of a skew heap is the merge operation which combines two skew heaps into a single, valid skew heap. Let `p1` and `p2` be positions in two skew heaps (e.g. pointers to nodes). The merge operation is defined recursively:

- If `p1` is Null, return `p2`; similarly, if `p2` is Null, return `p1`.
- Assume that `p1` has a higher priority than `p2`; if not, swap `p1` and `p2`.
- Swap the left and right subtrees of `p1`.
- Recursively merge `p2` and the left subtree of `p1`, replacing the left subtree of `p1` with the result of the recursive merge.

- The following figure shows two min-skew heaps and the result of merging the two. In a min-heap a smaller priority number means a higher priority node.
![image](https://github.com/user-attachments/assets/49f95863-5d1c-4ce1-b99b-bbc8c22612a0)
![image](https://github.com/user-attachments/assets/8cc01a96-019b-4b20-ab3c-c0aa2336819c)

### Leftist Heap

A leftist heap is similar to a skew heap and it uses a merge operation for insertion and deletion. However, it maintains a specific property and it uses the property during the merge operations. Every node in a leftist heap stores a value called Null Path Length (NPL). This value presents the lowest number of edges from the node to a null node. In other words, the NPL value represents the shortest possible path from a node to a null node.

During a merge operation, if the NPL value of the right child is larger than the NPL value of the left child, we swap the children. This generally makes the left subtree heavier than the right subtree.

## Assignment

In this project, you will implement a priority queue class based on a skew-heap or a leftist-heap data structure. It can maintain a min-heap or a max-heap based on the computed priority for every node, where the priority function is provided to the queue constructor via a function pointer. Inserting to and extracting from the skew/leftist heap uses a heap merge function which guarantees that the heap property (min-heap or max-heap) is maintained; the comparisons that are part of the merge process will be made on the computed priorities for the objects in the heap. The queue class allows for the priority function to be changed; in which case the heap must be rebuilt. Moreover, the data structure can switch between a skew heap or a leftist heap at the request of the class user. Such a request will trigger reconstruction of the heap.

For this project, you are provided with the following files:

- **mqueue.h** – The interface for the Order, Node, and MQueue classes.
- **mqueue.cpp** – A skeleton for the implementation of the class queue.
- **driver.cpp** – A sample driver program. It contains sample use of the queue class.
- **driver.txt** – A sample output produced by driver.cpp.

## Specifications

There are three classes in this project. The class `MQueue` has a member variable of type `Node`, and the class `Node` has a member variable of type `Order`.

### Class Order

The implementation of this class is provided to you. You are not allowed to modify the class. This class stores the following information about an order:

- **m_customer**: stores the customer's name. The name does not need to be unique.
- **m_FIFO**: stores the order's ordinal position in the list of orders. It shows how soon or how late the customer placed the order. The manufacturer keeps a maximum number of 50 orders in the list at any time. Position 1 indicates that the order was placed before every other order. The 50th position indicates that the order was the last one received.
- **m_processTime**: indicates how long it takes to complete the production of this order. The production can take between 1 to 12 months.
- **m_dueTime**: indicates when the order will be ready for delivery. It might be between 1 and 12 months.
- **m_slackTime**: is the remaining time until the due date minus the required time to complete the production. It might be between 1 and 12 months.
- **m_income**: stores the family income for the order. The possible values are defined in the enum type Income.
- **m_importance**: stores the importance of the customer to the manufacturer. The customers are rated from 1 to 100 percent. A value of 100 percent indicates the most important customer.
- **m_workForce**: stores the number of available personnel on the production line. This number varies between 100 and 200.
- **m_quantity**: stores the quantity of the order. The production line accepts orders of 1000 to 10000 units.

### Priority Functions

A priority function is a user-defined function that can use the information in the class `Order` to determine a priority value for an order. Two example functions are provided in the `driver.cpp` file. You can define your own priority functions. In fact, different plants may have different criteria for this purpose. The ability to use different functions allows us to adapt to different priority algorithms. The heap can accept a prioritization function. This is accomplished by passing a function pointer to the constructor. The function pointer is the address of a function that will be used to compute the priority for orders. The function must take an `Order` object as input and must return an integer priority value. A typedef for the function pointer is provided for you in the header file:

```cpp
typedef int (*prifn_t)(const Order&);
```
This says that prifn_t is a pointer to a function that takes an Order& argument.

- **priorityFn1(const Order & order)**: Determines a priority value for the argument and returns the priority value. The algorithm in this function uses the information in the `Order` class. In this algorithm, a greater value means a higher priority. To use this function, we need to build a max-heap. If the function cannot calculate a valid priority value, it returns zero.  
  *Note: The implementation of this function is provided to you. You do not need to modify it.*

- **priorityFn2(const Order & order)**: Returns the priority value for the argument. The algorithm in this function uses the information in the `Order` class. In this algorithm, a smaller value means a higher priority. To use this function, we need to build a min-heap. If the function cannot calculate a valid priority value, it returns zero.  
  *Note: The implementation of this function is provided to you. You do not need to modify it.*


### Class Node
This class constructs a node in the heap data structure. It has a member variable of the type Order. The member variable is initialized through the Node constructor. The class MQueue is a friend of Node class, it means it has access to private members of Node class. You are not allowed to modify this class.

### Overloaded Insertion Functions
There are two overloaded insertion functions for the classes Order and Node to help you debugging the project. The implementation is provided to you. You do not need to modify them.

### Class MQueue 

The class `MQueue` constructs a skew or a leftist data structure of the type min-heap or max-heap. This class has a member variable called `m_heap`. The member variable `m_heap` represents the root node of the heap data structure and it is of the type `Node`. The following table presents the list of member functions that need implementation.

- **MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);**  
  This is the constructor. It must be provided with a pointer to the prioritization function, the type of heap, and the desired data structure.

- **MQueue::~MQueue()**  
  The destructor deallocates the memory and re-initializes the member variables.

- **MQueue::MQueue(const MQueue& rhs)**  
  The copy constructor must make a deep copy of the `rhs` object. It must function correctly if `rhs` is empty. This function creates an exact copy of `rhs`.

- **MQueue& MQueue::operator=(const MQueue& rhs)**  
  The assignment operator creates an exact copy of `rhs`. You should not call the copy constructor in the implementation of the assignment operator.

- **bool MQueue::insertOrder(const Order& input)**  
  Inserts an order into the queue. It must maintain the min-heap or the max-heap property depending on the settings. Moreover, if the selected data structure is leftist heap, it needs to maintain a correct value of Null Path Length (NPL) in the node. If the order does not provide a valid priority value, the object is not inserted and the function returns `false`, otherwise the function returns `true`.

- **Order MQueue::getNextOrder()**  
  This function extracts (removes the node) and returns the highest priority order from the queue. It must maintain the min-heap or max-heap property. The function throws an `out_of_range` exception if the queue is empty when the function is called.

- **void MQueue::mergeWithQueue(MQueue& rhs)**  
  This function merges the host queue with the `rhs`; it leaves `rhs` empty; it transfers all nodes from `rhs` to the current heap. Two heaps can only be merged if they have the same priority function and are of the same data structure. If the user attempts to merge queues with different priority functions, or two different data structures, a `domain_error` exception should be thrown. This function requires protection against self-merging. Merging a heap with itself is not allowed.

- **void MQueue::clear()**  
  This function clears the queue. It must delete all the nodes in the heap, leaving the heap empty. Moreover, it re-initializes the member variables.

- **int MQueue::numOrders() const**  
  It returns the current number of orders in the queue.

- **void MQueue::printOrderQueue() const**  
  It prints the contents of the queue using preorder traversal. Although the first order printed should have the highest priority, the remaining orders will not necessarily be in priority order. Please refer to the sample output file (`driver.txt`) for the format of this function's output.

- **prifn_t MQueue::getPriorityFn() const**  
  This function returns the current priority function.

- **void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType)**  
  This function sets a new priority function and its corresponding heap type (min-heap or max-heap). It must rebuild the heap!  
  *Note: It is the responsibility of the caller to pass compatible arguments `priFn` and `heapType`.*

- **HEAPTYPE MQueue::getHeapType() const**  
  This function returns the heap type, i.e., it is either `MINHEAP` or `MAXHEAP`.

- **STRUCTURE MQueue::getStructure() const**  
  This function returns the structure of the current heap, i.e., it is either `SKEW` or `LEFTIST`.

- **void MQueue::setStructure(STRUCTURE structure)**  
  This function sets the data structure, i.e., it is either `SKEW` or `LEFTIST`. It must rebuild a heap with the new structure using the nodes in the current data structure.  
  *Note: Rebuild means transferring nodes, not recreating nodes.*

- **void MQueue::dump()**  
  This function prints out the node information in an in-order traversal. For every node, it prints the priority followed by the order's name of the node; and in the case of a leftist heap, the output is followed by the value of NPL for the node. The tree viewer tool shows a graphical representation of the output of the `dump` function. You can copy and paste the `dump()` output in the viewer. This tool facilitates debugging.  
  *Note: The implementation for this function is provided to you.*

## Additional Requirements

- Private helper functions must be declared in the header file. No other modifications to the header file are permitted!
- No STL containers or additional libraries may be used in the implementation of the project classes. However, you can use STL containers in the `Tester` class for the testing purposes.
- The required functionality is provided in the `Order` and `Node` classes. There is no need for any modifications to the implementation of these classes.
- Your code should not have any memory leaks or memory errors.
- In the case of a leftist heap, the lowest level of nodes that store the keys must have a zero NPL value.
- Computed priority values may not be pre-computed and stored with the `Order` object in the queue. They must be computed as needed using the priority function.
- Insertion to and extraction from the heap must run in amortized logarithmic time.
- Follow all coding standards as described on the C++ Coding Standards. In particular, indentations and meaningful comments are important.

## Testing

The test file name must be **mytest.cpp**; the file name must be in lowercase (e.g., `myTest.cpp` is not acceptable). The test file must contain the declaration and implementation of your `Tester` class and the `main()` function as well as all your test cases, i.e., calls to your test functions.

- You must write a separate function for every test case.
- Every test function must return true/false depending on passing or failing the test. Visual outputs are not accepted as test results.
- Tests cannot be interactive. The test file `mytest.cpp` must compile and run to completion.

## Testing MQueue Class

- Test insertion for a normal case of min-heap. After a decent number of insertions (e.g., 300 nodes), traverse the tree and check that the heap property is satisfied at every node.
- Test insertion for a normal case of max-heap. After a decent number of insertions (e.g., 300 nodes), traverse the tree and check that the heap property is satisfied at every node.
- Test removal for a normal case of min-heap. After a decent number of insertions (e.g., 300 nodes), check whether all removals happen in the correct order.
- Test removal for a normal case of max-heap. After a decent number of insertions (e.g., 300 nodes), check whether all removals happen in the correct order.
- Test that all nodes in a leftist heap have the correct NPL value.
- Test that a leftist heap preserves its property, i.e., at every node, the NPL value of the left child is greater than or equal to the NPL value of the right child.
- Test whether after changing the priority function, a correct heap is rebuilt with the same data (nodes) but a different priority function.
- Test the merge of an empty queue (an edge case) with a normal queue. This is a call to the function `MQueue::mergeWithQueue(MQueue& rhs)` where `rhs` is a normally populated queue.
- Test the `MQueue` class copy constructor for a normal case.
- Test the `MQueue` class copy constructor for an edge case.
- Test the `MQueue` class assignment operator for a normal case.
- Test the `MQueue` class assignment operator for an edge case.
- Test that attempting to dequeue an empty queue throws an `out_of_range` exception.
- Test that attempting to merge queues with different priority functions throws a `domain_error` exception.

## Random Numbers for Testing

For testing purposes, we need data. Data can be written as fixed values or can be generated randomly. Writing fixed data values might be tedious work since we need a large amount of data points. The approach for creating data will be your decision.

In the file `driver.cpp` there is the class `Random` which generates pseudorandom numbers. The class is using a default seed value. On the same machine, it always generates the same sequence of numbers. That is why the numbers are called pseudorandom numbers; they are not real random numbers. Please note, the numbers are machine-dependent, therefore, the numbers you see in the sample file `driver.txt` might be different from the numbers your machine generates.

## Memory Leaks and Errors

Run your test program in `valgrind`; check that there are no memory leaks or errors.  
*Note: If `valgrind` finds memory errors, compile your code with the `-g` option to enable debugging support and then re-run `valgrind` with the `-s` and `--track-origins=yes` options. Valgrind will show you the line numbers where the errors are detected and can usually tell you which line is causing the error.*
Never ignore warnings. They are a major source of errors in a program.

## What to Submit

You must submit the following files to the **proj3** directory:

- **mqueue.h**
- **mqueue.cpp**
- **mytest.cpp** (This file contains your `Tester` class, all test functions, all test cases, priority functions, and the `main` function.)

If you followed the instructions in the Project Submission page to set up your directories, you can submit your code using the following command:

```bash
cp mqueue.h mqueue.cpp mytest.cpp ~/cs341proj/proj3/
```

### Grading Rubric
The following presents a course rubric. It shows how a submitted project might lose points.

Conforming to coding standards make about 10% of the grade.
Your test program is worth 50%. If you submit the sample driver program as your test program or no test program is submitted there will be 50% deduction.
Correctness and completeness of your test cases (mytest.cpp) make about 15% of the grade.
We have written test cases to test your submission without knowing anything about your code. Therefore, it is extremely important that your submission conforms to the specified requirements. Passing tests make about 30% of the grade.
There is a 5% deduction for every modification that we need to perform to compile and run your work. For example, if we need to rename your file from myTest.cpp to mytest.cpp the deduction will be applied. One can find a non-exhaustive list of problems that cause deductions at the submission page.
If the submitted project is in a state that receives the deduction for all above items, it will be graded for efforts. The grade will depend on the required efforts to complete such a work.

