// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
// Helper to check if BST property is maintained using getters
bool isBSTPropertyMaintained(Robot* node, int min, int max) {
    if (node == nullptr) return true;
    int nodeID = node->getID();
    if (nodeID < min || nodeID > max) return false;
    return isBSTPropertyMaintained(node->getLeft(), min, nodeID - 1) &&
           isBSTPropertyMaintained(node->getRight(), nodeID + 1, max);
}

// Helper checks if the tree is balanced by comparing heights of left and right subtrees
bool isBalanced(Robot* node) {
    if (node == nullptr) return true;
    int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1;
    int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1;

    int balance = leftHeight - rightHeight;
    if (abs(balance) > 1) return false;

    return isBalanced(node->getLeft()) && isBalanced(node->getRight());
}
// Helper verifies if node heights are correctly calculated based on child heights
bool areHeightsCorrect(Robot* node) {
    if (node == nullptr) return true;
    
    int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1;
    int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1;

    // Ensure the current node's height is correctly calculated
    if (node->getHeight() != max(leftHeight, rightHeight) + 1) {
        cout << "Height mismatch at node with ID: " << node->getID() << "\n";
        return false;
    }

    // Recursively check for left and right subtrees
    return areHeightsCorrect(node->getLeft()) && areHeightsCorrect(node->getRight());
}

// Helper function to check if a tree is complete
bool isCompleteTree(Robot* root) {
    if (root == nullptr) return true; // Tree is empty

    // Array to simulate a queue with a simple size
    Robot* queue[100];
    int front = 0, rear = 0;
    bool emptySpotFound = false;

    queue[rear++] = root; // Start with root in queue

    while (front < rear) {
        Robot* current = queue[front++];

        if (current->getLeft() != nullptr) {
            if (emptySpotFound) return false; // Found an empty spot earlier
            queue[rear++] = current->getLeft();
        } else {
            emptySpotFound = true; // Mark empty spot
        }

        if (current->getRight() != nullptr) {
            if (emptySpotFound) return false; // Found an empty spot earlier
            queue[rear++] = current->getRight();
        } else {
            emptySpotFound = true; // Mark empty spot
        }
    }

    return true;
}

class Tester {
public:
    // Test cases
    bool testAVLTreeBalancing();
    bool testBSTPropertyAfterInsertions();
    bool testSplayOperations();
    bool testNormalRemoveInBST();
    bool testEdgeCaseRemoveInBST();
    bool testAVLTreeBalancingAfterRemovals();
    bool testBSTPropertyAfterRemovals();
    bool testAssignmentOperatorNormal();
    bool testAssignmentOperatorError();
    bool testChangeTreeTypeToAVL();
    bool testClearFunction();
    bool testCompleteTree();
    bool testFindMinMax();
};

bool Tester::testAVLTreeBalancing() {
    Swarm avl(AVL);
    Random idGen(MINID, MAXID);
    int insertCount = 300;
    for (int i = 0; i < insertCount; i++) {
        Robot robot(idGen.getRandNum());
        avl.insert(robot);
    }

    // Validate if the tree remains balanced
    if (isBalanced(avl.m_root) && areHeightsCorrect(avl.m_root)) {
        cout << "You PASSED the AVL Tree Balancing test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the AVL Tree Balancing test.\n";
        return false;
    }
}

bool Tester::testBSTPropertyAfterInsertions() {
    Swarm bst(BST);
    Random idGen(MINID, MAXID);
    int insertCount = 300;
    for (int i = 0; i < insertCount; i++) {
        Robot robot(idGen.getRandNum());
        bst.insert(robot);
    }

    // Check BST property using helper function
    if (isBSTPropertyMaintained(bst.m_root, MINID, MAXID)) {
        cout << "You PASSED the BST Property After Insertions test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the BST Property After Insertions test.\n";
        return false;
    }
}

bool Tester::testSplayOperations() {
    Swarm splayTree(SPLAY);
    Random idGen(MINID, MAXID);
    int insertCount = 100;
    for (int i = 0; i < insertCount; i++) {
        int id = idGen.getRandNum();
        Robot robot(id);
        splayTree.insert(robot);

        // Check if the inserted node is splayed to the root
        if (splayTree.m_root == nullptr || splayTree.m_root->getID() != id) {
            cout << "You DID NOT PASS the Splay Operations in Splay Tree test.\n";
            return false;
        }
    }

    // Check for BST property after all operations
    if (isBSTPropertyMaintained(splayTree.m_root, MINID, MAXID)) {
        cout << "You PASSED the Splay Operations in Splay Tree test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Splay Operations in Splay Tree test.\n";
        return false;
    }
}

bool Tester::testNormalRemoveInBST() {
    Swarm bst(BST);
    Random idGen(MINID, MAXID);
    Robot robot(idGen.getRandNum());
    bst.insert(robot);
    bst.remove(robot.getID());

    // Check if tree is empty by checking if root is null
    if (bst.m_root == nullptr) {
        cout << "You PASSED the Normal Remove in BST test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Normal Remove in BST test.\n";
        return false;
    }
}

bool Tester::testEdgeCaseRemoveInBST() {
    Swarm bst(BST);
    Robot robot(20000);
    bst.insert(robot);
    bst.remove(robot.getID());

    // Check if tree is empty by checking if root is null
    if (bst.m_root == nullptr) {
        cout << "You PASSED the Edge Case Remove in BST test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Edge Case Remove in BST test.\n";
        return false;
    }
}

bool Tester::testAVLTreeBalancingAfterRemovals() {
    Swarm avl(AVL);
    Random idGen(MINID, MAXID);
    int insertCount = 300;
    for (int i = 0; i < insertCount; i++) {
        Robot robot(idGen.getRandNum());
        avl.insert(robot);
    }
    int removeCount = 150;
    for (int i = 0; i < removeCount; i++) {
        int id = idGen.getRandNum();
        avl.remove(id);
    }

    // Validate if the tree remains balanced
    if (isBalanced(avl.m_root) && areHeightsCorrect(avl.m_root)) {
        cout << "You PASSED the AVL Tree Balancing After Removals test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the AVL Tree Balancing After Removals test.\n";
        return false;
    }
}

bool Tester::testBSTPropertyAfterRemovals() {
    Swarm bst(BST);
    Random idGen(MINID, MAXID);
    int insertCount = 300;
    for (int i = 0; i < insertCount; i++) {
        Robot robot(idGen.getRandNum());
        bst.insert(robot);
    }
    int removeCount = 150;
    for (int i = 0; i < removeCount; i++) {
        int id = idGen.getRandNum();
        bst.remove(id);
    }

    // Check BST property using helper function
    if (isBSTPropertyMaintained(bst.m_root, MINID, MAXID)) {
        cout << "You PASSED the BST Property After Removals test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the BST Property After Removals test.\n";
        return false;
    }
}

bool Tester::testAssignmentOperatorNormal() {
    Swarm swarm1(BST);
    Robot robot(30000);
    swarm1.insert(robot);
    Swarm swarm2;
    swarm2 = swarm1;

    // Check if the copy was successful by comparing root ids
    if (swarm2.getType() == swarm1.getType() && swarm2.findMin(swarm2.m_root)->getID() == robot.getID()) {
        cout << "You PASSED the Normal Assignment Operator test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Normal Assignment Operator test.\n";
        return false;
    }
}

bool Tester::testAssignmentOperatorError() {
    Swarm swarm1;
    Swarm swarm2;
    swarm2 = swarm1;

    // Check if both are still empty after assignment
    if (swarm2.m_root == nullptr && swarm1.m_root == nullptr) {
        cout << "You PASSED the Error Assignment Operator test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Error Assignment Operator test.\n";
        return false;
    }
}

bool Tester::testChangeTreeTypeToAVL() {
    Swarm bst(BST);
    Random idGen(MINID, MAXID);

    // Insert a few robots into the BST
    for (int i = 0; i < 10; i++) {
        Robot robot(idGen.getRandNum());
        bst.insert(robot);
    }

    // Change the tree type to AVL and check if the change is successful
    bst.setType(AVL);
    if (bst.getType() == AVL && isBalanced(bst.m_root)) {
        cout << "You PASSED the Change Tree Type to AVL test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Change Tree Type to AVL test.\n";
        return false;
    }
}

bool Tester::testClearFunction() {
    Swarm bst(BST);
    Random idGen(MINID, MAXID);

    // Insert a few robots
    for (int i = 0; i < 10; i++) {
        Robot robot(idGen.getRandNum());
        bst.insert(robot);
    }

    // Clear the tree
    bst.clear();

    // Check if the tree is empty
    if (bst.m_root == nullptr) {
        cout << "You PASSED the Clear Function test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Clear Function test.\n";
        return false;
    }
}


bool Tester::testCompleteTree() {
    Swarm bst(BST);
    // Inserting nodes that ensure a complete binary tree
    bst.insert(Robot(2));  // Root
    bst.insert(Robot(1));  // Left child of root
    bst.insert(Robot(3));  // Right child of root
    // Check if the tree is complete
    if (isCompleteTree(bst.m_root)) {
        cout << "You PASSED the Complete Tree test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Complete Tree test.\n";
        return false;
    }
}


bool Tester::testFindMinMax() {
    Swarm bst(BST);
    Random idGen(MINID, MAXID);
    // Insert a few robots
    for (int i = 0; i < 10; i++) {
        Robot robot(idGen.getRandNum());
        bst.insert(robot);
    }
    // Find the minimum and maximum nodes
    Robot* minNode = bst.findMin(bst.m_root);
    Robot* maxNode = bst.findMax(bst.m_root);
    // Check if the min and max are correct
    if (minNode != nullptr && maxNode != nullptr) {
        cout << "You PASSED the Find Min/Max test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Find Min/Max test.\n";
        return false;
    }
}


int main() {
    Tester tester;
    cout << "UNIT TEST - AVL Tree Balancing After Multiple Insertions\n";
    tester.testAVLTreeBalancing();
    cout << "UNIT TEST - BST Property After Multiple Insertions\n";
    tester.testBSTPropertyAfterInsertions();
    cout << "UNIT TEST - Splay Operations in Splay Tree\n";
    tester.testSplayOperations();
    cout << "UNIT TEST - Normal Remove in BST\n";
    tester.testNormalRemoveInBST();
    cout << "UNIT TEST - Edge Case Remove in BST (Single Node)\n";
    tester.testEdgeCaseRemoveInBST();
    cout << "UNIT TEST - AVL Tree Balancing After Multiple Removals\n";
    tester.testAVLTreeBalancingAfterRemovals();
    cout << "UNIT TEST - BST Property After Multiple Removals\n";
    tester.testBSTPropertyAfterRemovals();
    cout << "UNIT TEST - Assignment Operator (Normal Case)\n";
    tester.testAssignmentOperatorNormal();
    cout << "UNIT TEST - Assignment Operator (Error Case)\n";
    tester.testAssignmentOperatorError();
    cout << "UNIT TEST - Change Tree Type to AVL\n";
    tester.testChangeTreeTypeToAVL();
    cout << "UNIT TEST - Clear Function\n";
    tester.testClearFunction();
    cout << "UNIT TEST - Complete Tree\n";
    tester.testCompleteTree();
    cout << "UNIT TEST - Find Min and Max\n";
    tester.testFindMinMax();
    return 0;
}
