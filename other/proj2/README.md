[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/PAm4xzYE)
# CMSC341Proj2F23
Description for CMSC341 Project 2
<br>**You can find the project description on Blackboard
<body>
  <center><h1>
<p>Project 2: The Drone Show Software - <s>Spring 2023</s></p> 
<p>Fall 2023 (sections 99 & 100) Java Implementation</p>
<p>Due: Tuesday, Oct 17, before 11:59 pm</p>
</h1></center>
</body>
<br>
<h2>Objectives</h2>
<ul>
<li>Implementing balanced binary search tree (BST) data structures. </li>
	<li>Practice writing rebalancing routines.</li>
<li>Practice using recursion in programs. </li>
	<li>Practice writing unit tests.</li>
<li>Practice measuring the performance of an algorithm.</li>
</ul>
<h2>Introduction</h2>
<p>With the advances in drone technologies many companies today provide the large-scale show presentations of drones in the air. Some even argue that these types of shows can replace the fireworks. In such a show thousands of drones are flying synchronously and create different formations while flying. The following figure presents a sample formation.
<img src="https://npr.brightspotcdn.com/dims4/default/54e053d/2147483647/strip/true/crop/4096x2304+0+0/resize/1760x990!/format/webp/quality/90/?url=http%3A%2F%2Fnpr-brightspot.s3.amazonaws.com%2Fa0%2F5d%2Faebe606b4b959134ed6a095bc61e%2F500-drone-grid.jpg"style="width:500px;height:400px;></p>

<p>Figure from: https://www.wvxu.org/local-news/2022-08-25/light-up-drone-display-2022-blink-festival</p>
<p>In some configurations the drones are completely synchronized by the command center. And a team of pilots run the command center. In other configurations drones can perform some limited synchronization through peer-to-peer communication. In the other word, the drones send messages to each other. The communicated messages along with the current advances in the AI (Artificial Intelligence) field help the operations while reducing the communication between drones and the command center.</p>
<p>A startup company defined a project to develop a software module that runs in a drone and sends/receives the messages from other drones. You are assigned the task of designing and developing an efficient data structure that stores the information of all drones in a show. The software management team decided that a balanced Binary search tree would be the best option. The data structure that you develop will be loaded to all drones before the start of the show and at all times it will be updated through the communications between drones.</p>

<h2>The Binary Search Tree (BST)</h2>
<p>A binary tree is a tree structure in which each node has either 0, 1, or 2 children. A BST is a derivative of a binary tree where each node contains a key and value pair. The key determines the nodes' placement in the tree, and the value is the data to be stored. Given a set of rules about how to order the keys, we can create a structure where we can query data from it with a specified key. For a BST, we define these rules as follows:</p>
<p><ol>
<li>If the target key is less than the key at the current node, traverse to the left child.</li>
<li>If the target key is greater than the key at the current node, traverse to the right child.</li>
<li>If the keys are equal, the action is determined by our application of the tree. More on this later.</li>
</ol></p>
<p>A BST on its own can be efficient, but as the dataset increases in size, we can start running into problems. In the worst case, our BST can become a linked list where each of the new keys is greater than or less than the previous one inserted. On the contrary, the best case is inserting elements into the tree in a way to make it a complete tree. Either case is rare to occur with a large dataset, but imbalances are common. An imbalance can be defined when one subtree on a node becomes significantly larger in size or height compared to the other subtree. As the tree becomes increasingly imbalanced, our average query times begin to increase. Luckily, we have methods to prevent large imbalances.</p>
<br>
<h2>The AVL Tree</h2>
<p>An AVL tree employs rotations during insertions or deletions to balance a BST. As the name implies, nodes are literally rotated up the tree to keep its structure complete. A complete tree, or ideally a perfect tree, is the most efficient kind of binary tree. Insertions, deletions, and queries all take O(log(n)) time in such a case. AVL trees have two types of rotations, left and right, which are shown in the diagram below:</p>
<p><img src="https://redirect.cs.umbc.edu/~donyaee/341/current/projects/proj2files/rotations.png"></p>
<p>The variables "x" and "y" refer to 2 specific nodes whereas the subtrees "a", "b", and "c" refer to subtrees (which is just a pointer to a node which may or may not have more children). Note that the pointers to "a", "b", and/or "c" can be null, but "x" nor "y" will never be null.</p>
<p>The key to keeping an AVL tree efficient is when we perform these rotations. A rotation is performed on a node that is imbalanced, and an imbalance occurs when the node's children's heights differ by more than 1. For example, in the above diagram, consider node "y" to be imbalanced in the right rotation and node "x" to be imbalanced in the left rotation. Using a left and right rotation, we can perform four rotation combinations. The imbalance in the following examples occurs on the node with the height of 2 (in red).</p>
<p><ol>
<li>Single left rotation: This is a simple case where we can apply a left rotation to the top node to balance the tree.</li>
	<p><img src="https://redirect.cs.umbc.edu/~donyaee/341/current/projects/proj2files/single_left_rotation.png"></p>
<li>Single right rotation: Similar to the above case, we can apply a single right rotation to the top node to balance the tree.</li>
<img src="https://redirect.cs.umbc.edu/~donyaee/341/current/projects/proj2files/single_right_rotation.png">
 
<li>Double left-right rotation: The following two cases become more complicated and require two rotations. In this example, the imbalance still occurs at the node with height 2. If we perform a single right rotation, we still end up with an unbalanced tree, just mirrored (draw a diagram). So, we must perform two rotations. The first left rotation should transform the tree into a form we can balance with a second right rotation. Which node should the first rotation be performed on (hint: it's not necessarily the node with height 2)?</li>
<img src="https://redirect.cs.umbc.edu/~donyaee/341/current/projects/proj2files/left_right_rotation.png">
<li>Double right-left rotation: Likewise, this case uses a right rotation followed by a left rotation.</li>
<img src="https://redirect.cs.umbc.edu/~donyaee/341/current/projects/proj2files/right_left_rotation.png"></ol></p>

<h2>Assignment</h2>
<p>Your assignment is to implement a binary search tree with balancing methods.</p>
<p>For this project, you are provided with the skeleton .java files:</p>
<p><ul>
<li>Show.java– A skeleton for the implementation of the class Show</li>
<li>myShow.java – A skeleton for the Junit test of class Show</li>
<li>ShowTestPerformance.java – Junit test to show performance of class Show</li>
<li>RandomGen.java – Class for generating random numbers, used for testing</li>
</ul></p>
<p>Please note, you may not change any of the private variables or public function declarations or file names. They will be used for grading. Also, any provided function implementations may not be modified. You may, however, add your own private variables and functions as helpers. The current private function declarations are provided as a backbone to help you.</p>
<p>Additionally, you are responsible for thoroughly testing your program. Your test program, myShowTest.java, must be a Junit test and must be submitted along with other files. For grading purposes, your implementation will be tested on input data of varying sizes, including very large data. <s>Your submission will also be checked for memory leaks and memory errors.</s></p>

<h2>Specifications</h2>
<p>This project has three classes: RandomGen, Drone, and Show. The RandomGen class is provided as a supplementary class to facilitate the testing. The Drone class defines the nodes in the binary tree. The Show class is the one that implements the balanced binary search tree.</p>
<h2>Class Show</h2>
<p>The Show class implements an AVL BST. The m_root member variable points to a node of type Drone. Every node in Show is a Drone object. The nodes are organized as a binary search tree. The Show class supports the insertion and deletion operations. After insertion or deletion operations the class checks for any imbalance and if required it performs the rebalancing operations.
For the Show class, you must implement the following methods in Show.java:</p>
<table>
	<tr>
		<td>Show()</td>
		<td>The constructor performs the required initializations.</td>
	</tr>
	<tr>
		<td>Show(Show rhs)</td>
		<td>Copy constructor. It creates an exact deep copy of the rhs.</td>
	</tr>
	<tr>
		<td><s>~Show()</s></td>
		<td><s>The destructor performs the required cleanup including memory deallocations and re-initializing</s></td>
	</tr>
	<tr>
		<td>void insert(const Drone aDrone)</td>
		<td>This function inserts a Drone object into the tree in the proper position. The Drone’s m_id should be used as the key to traverse the Show tree and abide by BST traversal rules. The comparison operators (>, <, ==, !=) work with the int type in Java. A Drone id is a unique number in the range MINID - MAXID. We do not allow a duplicate id or an object with invalid id in the tree. Note:
In the Show tree data structure every node is a Drone object which is represented as a pointer reference to the Drone object. Therefore, for every insertion we need to allocate memory and use the information of aDrone to initialize the new node. Memory allocation takes place in the Show class. After an insertion, we should also update the height of each node on the path traversed down the tree as well as check for an imbalance at each node in this path.</td>
	</tr>
	<tr>
		<td>void clear()</td>
		<td>The clear function <s>deallocates all memory in the tree and</s>
makes it an empty tree.</td>
	</tr>
	<tr>
		<td>void remove(int id)</td>
		<td>The remove function traverses the tree to find a node with
the id and removes it from the tree.</td>
	</tr>
	<tr>
		<td>void listDrones()</td>
		<td>This function returns a list of all drones in the tree in the ascending order of IDs. The information for every Drone object will be placed in a new line. For the format of output please refer to the myShowTest for sample output</td>
	</tr>
	<tr>
		<td>bool setState(int id, STATE state)</td>
		<td>This function finds the node with id in the tree and sets its m_state member variable to state. If the operation is successful, the function returns true otherwise it returns false. For example, when the drone with id does not exist in
the tree the function returns false.</td>
	</tr>
	<tr>
		<td>void removeLightOff()</td>
		<td>This function traverses the tree, finds all drones with LIGHTOFF state and removes them from the tree. The final
tree must be a balanced AVL tree.</td>
	</tr>
	<tr>
		<td>void removeLightOff()</td>
		<td>This function traverses the tree, finds all drones with LIGHTOFF state and removes them from the tree. The final
tree must be a balanced AVL tree.</td>
	</tr>
	<tr>
		<td>int countDrones(LIGHTCOLOR aColor) const</td>
		<td>This function traverses the tree, finds all drones with the color specified by aColor variable and returns the total
number of drones of that color in the show.</td>
	</tr>
	<tr>
		<td>bool findDrone(int id) const</td>
		<td>This function returns true if it finds the node with id in the
tree, otherwise it returns false.</td>
	</tr>
	<tr>
		<td><s>const Show & Show::operator=(const Show & rhs)</s></td>
		<td><s>This function overloads the assignment operator for the class
Show. It creates an exact deep copy of the rhs.</s></td>
	</tr>
</table>

<h2>Additional Requirements</h2>
<p><ul>
<li>The class declarations Show, Drone and RandomGen are provided function implementations in Show.java may not be modified in any way. No additional libraries may be used. However, private helper functions are permitted in the Show class.</li>
<li>No STL containers or additional libraries may be used in the implementation of the Show class. However, you can use STL containers in the myShowTest class for testing purposes.</li>
<li>The required functionality is provided in the Drone class. There is no need for any modifications to the implementation of this class.</li>
<li><s>Your code should not have any memory leaks or memory errors.</s></li> 
	<li>The lowest level of nodes which store the keys have zero height.</li>
<li>Follow all coding standards as described on the C++ Coding Standards. In particular, indentations and meaningful comments are important.</li>
<li>The function dumpTree(...) prints out the nodes information in an in-order traversal. For every node, it prints the id followed by the height of the node in the Show tree. The following example presents a sample output of the dumpTree() function. The <a href="https://swe.umbc.edu/~donyaee/res/tree_viewer.html">tree viewer tool</a> shows a graphical representation of the output of the dump function. You can copy & paste the dump() output in the viewer. This tool facilitates debugging. Note: The implementation for this requirement is provided to you.</ul></p>
<div>((((19224:0)19289:1(19372:0))20201:2((26241:0)27904:1))53002:3(60496:1(93209:0)))</div>

<img src="https://redirect.cs.umbc.edu/~donyaee/341/current/projects/proj2files/dump-example.png" style="width:500px;height:250px;">

 


<h2>Testing</h2>
<p>You need to test your project and you need to submit your tests along with your project. Tests must be submitted in a file called
myShowTest.java.</p>
<ul>
<li>The test file name must be myShowTest.java; the file name must be in lower case, a file name like myTest.java is not acceptable. The test file must be a Junit test.</li>
<li>You are responsible for thoroughly testing your work before submission. The following section presents a non-exhaustive list of tests to perform on your implementation.</li>
<li>You must write a separate test function for every test case.</li>
<li>Tests cannot be interactive. The test file myShowTest.java must compile and run to completion.</li>
<li>The testing guidelines page provides information that helps you to write more effective test cases.</li>
</ul>
<p>Note: Testing incrementally makes finding bugs easier. Once you finish a function and it is testable, make sure it is working correctly.</p>
<h2>Testing Show Class</h2>
<ul>
<li>Test the insertion function for a normal case.</li> 
	<li>Test the insertion function for an edge case. Test the insertion function for an error case.</li>
<li>Test whether the tree is balanced after a decent number of insertions, e.g. 300 insertions. (Note: this requires visiting all nodes and checking the height values)</li>
<li>Test whether the BST property is preserved after all insertions. (Note: this requires visiting all nodes and comparing key values) Test the remove function for a normal case.</li>
<li>Test the remove function for an edge case.</li>
<li>Test whether the tree is balanced after multiple removals. For example, insert 300 nodes, then remove 150, and check the property.</li>
<li>Test whether the BST property is preserved after multiple removals. Test the removeLightOff() functionality.</li>
<li>Test the countDrones(...) functionality.</li>
<li>Test the findDrone(...) functionality for a normal case. </li>
<li>Test the findDrone(...) functionality for a normal case. </li>
	<li>Test the copy constructor</li>
 <li>Test the assignment operator for a normal case.</li>
<li><s>Test the assignment operator for an error case, e.g. assigning an empty object to an empty object.</s></li>
<li>Prove that the removal operation performs in O(log n). For a sample code of time measurement please refer to the ShowTestPerformance.java</li>
<li>Prove that the insertion performs in O(log n). The following explains a sample algorithm to check the performance.</li>
	       <ul>
          <li>
            <p>The insertion time for an AVL tree is log n where n is the number of nodes. Therefore, constructing a tree of n nodes (inserting n nodes) would take n log n time. We can measure the insertion time for constructing two trees with 1000 and 2000 nodes and determine the ratio of change in time while we know that we doubled the number of insertions. For the simplicity we can use the number of ticks instead of actual time.</p>
          </li>
          <li>
            <pre class="code">
n = 1000
T1 = n log n				time for constructing a tree of 1000 nodes
T2 = 2n log 2n				time for constructing a tree of 2000 nodes 
T2 / T1 = 2n log 2n / n log n		time ratio
T2 / 2T1 = 2n log 2n / 2n log n
T2 / 2T1 = log 2n / log n = log 2000 / log 1000 = 1.1
T2 / 2T1 = 1.1				the ratio we expect theoretically
We consider an absolute error of 0.4 to account for variability in execution environment, etc.
(1.1 – 0.4) < T2 / 2T1 < (1.1 + 0.4)	this ratio is an acceptable ratio
This absolute error value is generally valid when you run your project on the GL server.
The error might be larger on your own computer due to variations in hardware and software 
  architecture on your computer. 
            </pre>
          </li>
        </ul>
</ul>


<h2>Random Numbers for Testing</h2>
<p>For testing purposes, we need data. Data can be written as fixed values or can be generated randomly. Writing fixed data values might be a tedious work since we need a large amount of data points. The approach for creating data will be your decision.</p>
<p>The file ShowTestPerformance.java uses the class RandomGen which generates pseudorandom numbers. The class is using a default seed value. On the same machine it always generates the same sequence of numbers. That is why the numbers are called pseudorandom numbers, they are not real random numbers. Please note, the numbers are machine dependent.</p>
<h2>Memory leaks and errors</h2>
<ul>
<li><s>Run your test program in valgrind; check that there are no memory leaks or errors.
Note: If valgrind finds memory errors, compile your code with the -g option to enable debugging support and then re-run valgrind with the -s and --track-origins=yes options. valgrind will show you the lines numbers where the errors are detected and can usually tell you which line is causing the error.</s></li>
<li>Never ignore warnings. They are a major source of errors in a program.</li>
</ul>
<h2>Implementation Notes</h2>
<ul>
<li>Implement incrementally based on the dependencies between the functions in a class. </li>
	<li>It'll be much more convenient if you first come up with a development plan.</li>
<li>Using helper functions is a convenient way to implement recursive functionality.</li>
<li>The RandomGen class is provided for you. This class allows for generating a large amount of random numbers for testing purposes. You can use this class for myShowTest.java.</li>
</ul>
<h2>What to Submit</h2>
<p>Nothing to upload to Blackboard, we will be using your github classroom repository</p>
<p><s>If you followed the instructions in the Project Submission page to set up your directories, you can submit your code using the following command:</s></p>
<pre class="code">   <s>cp show.h show.cpp mytest.cpp ~/cs341proj/proj2/</s></pre>


<h2>Grading Rubric</h2>
<p>The following presents a course rubric. It shows how a submitted project might lose points.</p>
<ul>
<li>Conforming to coding standards make about 10% of the grade.</li>
<li>Correctness and completeness of your test cases (myShowTest.cpp) make about 15% of the grade.</li>
<li>We have written test cases to test your submission without knowing anything about your code. Therefore, it is extremely important that your submission conforms to the specified requirements. Passing tests make about 75% of the grade.</li>
 
<li>There is a 5% deduction for every modification that we need to perform to compile and run your work. For example, if we need to rename your file from myShowtest.java to myShowTest.cpp the deduction will be applied.</li>
<li>There must be a reasonable number of commits in your github classroom repository. You can make commits to your local directory as often as you want. You would want to commit to your github classroom repository every time you have developed a new feature, fixed a bug, etc. You should be thinking dozens of commits. If your commit history is not convincing enough to determine that you did the development yourself, you could lose up to 50% of your grade.</li>
</ul>

