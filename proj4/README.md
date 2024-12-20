# CMSC 341 - Project 4: A File System - Fall 2024
## Due: Tuesday, December 3, before 9:00 PM

### Objectives
- To implement a hash table.
- To implement linear, quadratic, and double-hash probing to manage hash collisions.
- To implement an incremental re-hashing algorithm.
- To practice use of function pointers.
- To practice writing unit tests.
- To practice working in a Linux environment.
- To practice analyzing and understanding project requirements.

### Introduction
We are implementing a new operating system. A part of this operating system is a file system to manage the files on the disk. Users can create a new file, delete an existing file, and search for a file. The main operations in the file system are insert, find, and remove. Since the efficiency of operations is extremely important, we have decided to use a hash table to manage accessing the files on the disk. The keys are file names, which can cause collisions and clustering in a hash table, since there are many common names. To manage the collision cases, we use multiple forms of probing for the collision handling policy. The file system can change the table size based on some specific criteria in order to use the memory more efficiently. When a change is required, we need to rehash the entire table. However, the rehash operation happens incrementally during multiple regular operations.

### Hash Tables
A hash table is a data structure which implements the map ADT. It stores pairs of key-value. The key determines the index number of an array cell (bucket) in which the value will be stored. A hash function calculates the index number. If there is more than one value for a key in the data set, the hash table uses a collision handling scheme to find another cell for storing the new value.

#### Linear Probing
When we try to find an index in the hash table for a key and we find out that the determined bucket is taken, we need to find another bucket to store the key. In linear probing, we store the information in the next available bucket. In this scheme, the jump step to find the next available bucket is one.

#### Quadratic Probing
Linear probing can cause clustering, i.e., having many data points in a row. Clustering reduces the search efficiency. Using quadratic probing, the jump step to find the next available bucket is the square of the iteration number.

#### Double-Hash Probing
In this project, we use the following equation to find the next bucket for the purpose of the double hashing collision handling, where i = 0, 1, 2, 3, ...:
**index = ((Hash(key) % TableSize) + i x (11 - (Hash(key) % 11))) % TableSize**


### Assignment
Your assignment is to complete the class FileSys and write the appropriate test functions.

The application starts with a hash table of size MINPRIME. After certain criteria appear, it will switch to another table and it transfers all data nodes from the current table to the new one incrementally. Once the switching process starts, it scans 25% of the table and transfers any live nodes it finds in the old table, and at every consecutive operation (insert/remove), it continues to scan 25% more of the table and transfers live data from the old table to the new table until all data is transferred. We do not transfer deleted buckets to the new table.

After an insertion, if the load factor becomes greater than 0.5, we need to rehash to a new hash table. The capacity of the new table would be the smallest prime number greater than 4 times the current number of data points. The current number of data points is the total number of occupied buckets minus the total number of deleted buckets.

After a deletion, if the number of deleted buckets is more than 80 percent of the total number of occupied buckets, we need to rehash to a new table. The capacity of the new table would be the smallest prime number greater than 4 times the current number of data points.

During a rehashing process, the deleted buckets will be removed from the system permanently. They will not be transferred to the new table.

If a change of collision handling policy is requested by the user while the program is running, the new policy will be applied to the new table when a new rehash is triggered.

For this project, you are provided with the following files:
- `filesys.h` – header file for the File and FileSys classes.
- `filesys.cpp` – the template file for the FileSys class. Complete your FileSys implementation in this file.
- `driver.cpp` – A sample driver program.
- `driver.txt` – A sample output produced by driver.cpp.

### Specifications

#### FileSys Class
The FileSys class uses the File class. It has a member variable to store a pointer to a hash function. It also has two member variables to store pointers to two arrays of File objects. These arrays are `m_currentTable` and `m_oldTable`, and the `m_name` member variable of the File object is used as the key for hashing purposes. A File object has another member variable which stores a file block number on disk. The file block number and the file name define the uniqueness of a File object together.

##### FileSys::FileSys(int size, hash_fn hash, prob_t probing = DEFPOLCY)
The constructor takes size to specify the length of the current hash table, and hash is a function pointer to a hash function. The type of hash is defined in FileSys.h.
The table size must be a prime number between MINPRIME and MAXPRIME. If the user passes a size less than MINPRIME, the capacity must be set to MINPRIME. If the user passes a size larger than MAXPRIME, the capacity must be set to MAXPRIME. If the user passes a non-prime number, the capacity must be set to the smallest prime number greater than the user's value. The probing parameter specifies the type of collision handling policy for the current hash table.
Moreover, the constructor creates memory for the current table and initializes all member variables.

##### FileSys::~FileSys()
Destructor deallocates the memory.

##### bool FileSys::insert(File file)
This function inserts an object into the current hash table. The insertion index is determined by applying the hash function `m_hash` that is set in the FileSys constructor and then reducing the output of the hash function modulo the table size. A sample hash function is provided in the driver.cpp file.
Hash collisions should be resolved using the probing policy specified in the `m_currProbing` variable. We insert into the table indicated by `m_currentTable`. After every insertion, we need to check for the proper criteria, and if it is required, we need to rehash the entire table incrementally into a new table. The incremental rehashing proceeds with scanning 25% of the table at a time and transfers any live data found to the new table. Once we transferred the live nodes in the first 25% of the table, the second 25% live data will be transferred at the next operation (insertion or removal). Once all data is transferred to the new table, the old table will be removed, and its memory will be deallocated.
If the File object is inserted, the function returns true, otherwise it returns false. A File object can only be inserted once. The hash table does not contain duplicate objects. Moreover, the block number value should be a valid one falling in the range [DISKMIN-DISKMAX]. Every File object is a unique object carrying the File's name and the file block number. The File's name is the key which is used for hashing.

##### bool FileSys::remove(File file)
This function removes a data point from either the current hash table or the old hash table where the object is stored. In a hash table, we do not empty the bucket, we only tag it as deleted. To tag a removed bucket, we can use the member variable `m_used` in the File class. To find the bucket of the object, we should use the proper probing policy for the table.
After every deletion, we need to check for the proper criteria, and if it is required, we need to rehash the entire table incrementally into the current table. The incremental rehashing proceeds with scanning 25% of the table at a time and transfer any live data found to the new table. Once we transferred the live nodes in the first 25% of the table, the second 25% live data will be transferred at the next operation (insertion or removal). Once all data is transferred to the new table, the old table will be removed, and its memory will be deallocated.
If the File object is found and is deleted, the function returns true, otherwise it returns false.

##### File FileSys::getFile(string name, int block) const
This function looks for the File object with the name and the file block number in the database, if the object is found the function returns it, otherwise the function returns an empty object.

##### bool updateDiskBlock(File file, int block)
This function looks for the File object in the database, if the object is found the function updates its block number and returns true, otherwise the function returns false.

##### float FileSys::lambda() const
This function returns the load factor of the current hash table. The load factor is the ratio of occupied buckets to the table capacity. An occupied bucket is a bucket which can contain either a live data node (available to be used) or a deleted node.

##### float FileSys::deletedRatio() const
This function returns the ratio of the deleted buckets to the total number of occupied buckets.

##### void changeProbPolicy(prob_t policy)
The user can change the collision handling policy of the hash table at runtime. If a change request has been submitted by a user, the new policy will be stored in the variable `m_newPolicy`. Once the next rehash operation is initiated, the new policy will be used for the newly created hash table.

##### void FileSys::dump()
This function dumps the contents of the current hash table and the old hash table if it exists. It prints the contents of the hash table in array-index order. Note: The implementation of this function is provided. The function is provided to facilitate debugging.

##### int FileSys::findNextPrime(int current)
This function returns the smallest prime number greater than the argument "current". If "current" is less than or equal to MINPRIME, the function returns MINPRIME. If "current" is greater than or equal to MAXPRIME, the function returns MAXPRIME. In a hash table, we'd like to use a table with prime size. Then, every time we need to determine the size for a new table, we use this function. Note: The implementation of this function is provided.

##### bool FileSys::isPrime(int number)
This function returns true if the passed argument "number" is a prime number, otherwise it returns false. Note: The implementation of this function is provided.

### Additional Requirements
- Private helper functions may be added to the FileSys class; however, they must be declared in the private section of the class declaration.
- Every table may have its own collision handling policy. If previously a change of policy has been requested and then a rehash initiates, the new table will use the new policy and the old table will use its own policy while it exists.
- The allocated memory to the hash table must be dynamically managed at execution time when there is rehashing.
- Once the current hash table exceeds some criteria, the FileSys class rehashes the data into a new hash table. This requires creating a new table and swapping the two tables, so the newly created table becomes the current table.
- The capacity of the new table is determined by the information from the current table (which will become the old table). It would be the smallest prime number greater than ((m_currentSize - m_numDeleted)*4).
- For rehashing, we scan 25% of the table at every operation and transfer any live data to the new table. The class FileSys has a member variable named `m_transferIndex` which can be used to keep track of the current status of transfer.
- The 25% of data is an integer number, we use the floor value of the result.
- When rehashing, the deleted buckets will be removed from the system.
- The insertion of data points always happens in the current table.
- The find operation can happen in the current table and the old table if it exists.
- The remove operation can happen in the current table and the old table if it exists.
- Once the incremental rehashing ends and there are no more live data objects in the old table, the old table must be removed and its memory must be deallocated.

Here are the rules for lazy deletion:
- Treat deleted element as empty when inserting.
- Treat deleted element as occupied when searching.

Here are the rules for rehashing criteria:
- Rehash once the load factor exceeds 50%.
- Rehash once the deleted ratio exceeds 80%.
- The load factor is the number of occupied buckets divided by the table size. The number of occupied buckets is the total of available data and deleted data.
- The deleted ratio is the number of deleted buckets divided by the number of occupied buckets.
- No STL containers or additional libraries may be used in the FileSys class. STL containers can be used in `mytest.cpp` for testing purposes.

### Testing
- The test file name must be `mytest.cpp`; the file name must be in lower case, a file name like `myTest.cpp` is not acceptable.
- The test file must contain the declaration and implementation of your Tester class and the main() function as well as all your test cases, i.e., calls to your test functions.
- You are responsible for adequately testing your work before submission. The following section presents a non-exhaustive list of tests to perform on your implementation.
- You must write a separate function for every test case.
- Every test function must return true/false depending on passing or failing the test. Visual outputs are not accepted as test results.
- The `dump()` function should not be called in the test functions. The `dump()` function is only provided to facilitate debugging.
- Tests cannot be interactive. The test file `mytest.cpp` must compile and run to completion.
- An example of declaring, implementing, and calling a test function, and outputting the test results was provided in the driver.cpp file of project 0.
- The testing guidelines page provides information that helps you to write more effective test cases.
- Note: Testing incrementally makes finding bugs easier. Once you finish a function and it is testable, make sure it is working correctly.

### Testing FileSys Class
- Test the insertion operation in the hash table. The following presents a sample algorithm to test the normal insertion operation:
  - There are some non-colliding data points in the hash table.
  - Insert multiple non-colliding keys.
  - Check whether they are inserted in the correct bucket (correct index).
  - Check whether the data size changes correctly.
- Test the find operation (`getFile(...)` function) for an error case, the File object does not exist in the database.
- Test the find operation (`getFile(...)` function) with several non-colliding keys.
- Test the find operation (`getFile(...)` function) with several colliding keys without triggering a rehash. This also tests whether the insertion works correctly with colliding data.
- Test the remove operation with a few non-colliding keys.
- Test the remove operation with a number of colliding keys without triggering a rehash.
- Test the rehashing is triggered after a descent number of data insertion.
- Test the rehash completion after triggering rehash due to load factor, i.e., all live data is transferred to the new table and the old table is removed.
- Test the rehashing is triggered after a descent number of data removal.
- Test the rehash completion after triggering rehash due to delete ratio, i.e., all live data is transferred to the new table and the old table is removed.

#### Random Numbers for Testing
For testing purposes, we need data. Data can be written as fixed values or can be generated randomly. Writing fixed data values might be a tedious work since we need a large amount of data points. Please note, testing with two or three data points is not an acceptable approach. Testing must be performed using an appropriate number of data points.

In the file `driver.cpp` there is the class Random which generates pseudorandom numbers. The class is using a default seed value. It also is possible to change the seed value. On the same machine, it always generates the same sequence of numbers. That is why the numbers are called pseudorandom numbers, they are not real random numbers. Please note, the numbers are machine-dependent, therefore, the numbers you see in the sample file `driver.txt` might be different from the numbers your machine generates.

#### Testing For Memory Leaks / Memory Errors
- Run your test program in valgrind; check that there are no memory leaks or errors.
- Note: If valgrind finds memory errors, compile your code with the -g option to enable debugging support and then re-run valgrind with the -s and --track-origins=yes options. valgrind will show you the line numbers where the errors are detected and can usually tell you which line is causing the error.
- Never ignore warnings. They are a major source of errors in a program.

### What to Submit
You must submit the following files to the proj4 submit directory:
- `filesys.h`
- `filesys.cpp`
- `mytest.cpp` (Note: This file contains the declaration and implementation of your Tester class as well as all your test cases.)

If you followed the instructions in the Project Submission page to set up your directories, you can submit your code using the following command:

```bash
cp filesys.h filesys.cpp mytest.cpp ~/cs341proj/proj4/
```
### Grading Rubric
The following presents a course rubric. It shows how a submitted project might lose points.

- Conforming to coding standards make about 10% of the grade.
- Your test program is worth 50%. If you submit the sample driver program as your test program or no test program is submitted, there will be a 50% deduction.
- Correctness and completeness of your test cases (`mytest.cpp`) make about 15% of the grade.
- We have written test cases to test your submission without knowing anything about your code. Therefore, it is extremely important that your submission conforms to the specified requirements. Passing tests make about 30% of the grade.
- There is a 5% deduction for every modification that we need to perform to compile and run your work. For example, if we need to rename your file from `myTest.cpp` to `mytest.cpp`, the deduction will be applied. One can find a non-exhaustive list of problems that cause deductions at the submission page.
  
If the submitted project is in a state that receives the deduction for all above items, it will be graded for efforts. The grade will depend on the required efforts to complete such a work.


