// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
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


/* Priority functions */
int priorityFn1(const Order & order) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1101-10400]
    int minValue = MIN1000 + MAX100 + MINONE + MINZERO;
    int maxValue = MAX100 + MAX100 + MAX200 + MAX10000;
    //the larger value means the higher priority
    int priority = order.getMaterial() + order.getImportance() + 
                    order.getWorkForce() + order.getQuantity();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

int priorityFn2(const Order & order) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [4-86]
    int minValue = MINONE + MINONE + MINONE + MINONE;
    int maxValue = MAX12 + MAX12 + MAX12 + MAX50;
    //the smaller value means the higher priority
    int priority = order.getFIFO() + order.getProcessTime() + 
                order.getDueTime() + order.getSlackTime();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

class Tester {
public:
    // test cases
    bool testInsertMinHeap();
    bool testInsertMaxHeap();
    bool testRemoveMinHeap();
    bool testRemoveMaxHeap();
    bool testLeftistHeapNPL();
    bool testLeftistHeapProperty();
    bool testPriorityChange();
    bool testMergeWithEmptyQueue();
    bool testCopyConstructor();
    bool testCopyConstructorEdge();
    bool testAssignmentOperatorNormal();
    bool testAssignmentOperatorEdge();
    bool testEmptyDequeueException();
    bool testIncompatibleMerge();
};

bool Tester::testInsertMinHeap() {
    MQueue queue(priorityFn2, MINHEAP, SKEW);
    cout << "UNIT TEST - Insert into Min-Heap\n";

    Order order1("John", 10, 5, 3, 2, 80, 90, 150, 5000);
    Order order2("Doe", 5, 4, 2, 1, 90, 85, 160, 6000);
    queue.insertOrder(order1);   // insert first order
    queue.insertOrder(order2);   // insert second order

    if (queue.getNextOrder().getCustomer() == "Doe") {
        cout << "You PASSED the Insert into Min-Heap test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Insert into Min-Heap test.\n";
        return false;
    }
}

bool Tester::testInsertMaxHeap() {
    MQueue queue(priorityFn1, MAXHEAP, LEFTIST);
    cout << "UNIT TEST - Insert into Max-Heap\n";

    Order order1("Alice", 20, 6, 8, 5, 75, 95, 140, 4500);
    Order order2("Bob", 15, 3, 7, 6, 70, 92, 130, 4000);
    queue.insertOrder(order1);   // insert first order
    queue.insertOrder(order2);   // insert second order

    if (queue.getNextOrder().getCustomer() == "Alice") {
        cout << "You PASSED the Insert into Max-Heap test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Insert into Max-Heap test.\n";
        return false;
    }
}

bool Tester::testRemoveMinHeap() {
    MQueue queue(priorityFn2, MINHEAP, SKEW);
    cout << "UNIT TEST - Remove from Min-Heap\n";

    Random nameGen(97, 122);
    for (int i = 0; i < 10; i++) {
        Order order(nameGen.getRandString(5), i + 1, 4, 5, 3, 60, 70, 150, 3000);
        queue.insertOrder(order); // insert multiple orders with increasing FIFO
    }

    if (queue.getNextOrder().getFIFO() == 1) {
        cout << "You PASSED the Remove from Min-Heap test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Remove from Min-Heap test.\n";
        return false;
    }
}

bool Tester::testRemoveMaxHeap() {
    MQueue queue(priorityFn1, MAXHEAP, LEFTIST);
    cout << "UNIT TEST - Remove from Max-Heap\n";

    // insert 300 orders with ascending FIFO values
    for (int i = 1; i <= 300; i++) {
        Order order("Order" + to_string(i), i, 4, 5, 3, 60, 70, 150, 3000);
        queue.insertOrder(order);
    }

    // check if each removed order has the correct max FIFO value
    for (int expectedFIFO = 300; expectedFIFO >= 1; expectedFIFO--) {
        if (queue.getNextOrder().getFIFO() != expectedFIFO) {
            cout << "You DID NOT PASS the Remove from Max-Heap test.\n";
            return false;
        }
    }

    cout << "You PASSED the Remove from Max-Heap test.\n";
    return true;
}



bool Tester::testLeftistHeapNPL() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    cout << "UNIT TEST - Leftist Heap NPL Verification\n";

    Order order1("test", 1, 4, 5, 3, 60, 70, 150, 3000);
    queue.insertOrder(order1);
    
    if (queue.getStructure() == LEFTIST) {
        cout << "You PASSED the Leftist Heap NPL Verification test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Leftist Heap NPL Verification test.\n";
        return false;
    }
}

bool Tester::testLeftistHeapProperty() {
    MQueue queue(priorityFn2, MINHEAP, LEFTIST);
    cout << "UNIT TEST - Leftist Heap Property Verification\n";

    Order order1("order", 1, 3, 3, 2, 50, 60, 200, 4000);
    queue.insertOrder(order1);

    if (queue.getStructure() == LEFTIST) {
        cout << "You PASSED the Leftist Heap Property Verification test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Leftist Heap Property Verification test.\n";
        return false;
    }
}

bool Tester::testPriorityChange() {
    MQueue queue(priorityFn2, MINHEAP, SKEW);
    cout << "UNIT TEST - Priority Change\n";

    queue.setPriorityFn(priorityFn1, MAXHEAP); // change to new priority function
    queue.insertOrder(Order("test", 3, 2, 1, 1, 80, 90, 150, 2000));

    if (queue.getHeapType() == MAXHEAP) {
        cout << "You PASSED the Priority Change test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Priority Change test.\n";
        return false;
    }
}

bool Tester::testMergeWithEmptyQueue() {
    MQueue queue(priorityFn2, MINHEAP, SKEW);
    MQueue emptyQueue(priorityFn2, MINHEAP, SKEW);
    cout << "UNIT TEST - Merge with Empty Queue\n";

    queue.mergeWithQueue(emptyQueue);

    if (emptyQueue.numOrders() == 0 && queue.numOrders() > 0) {
        cout << "You PASSED the Merge with Empty Queue test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Merge with Empty Queue test.\n";
        return false;
    }
}

bool Tester::testCopyConstructor() {
    MQueue queue(priorityFn2, MINHEAP, SKEW);
    MQueue copiedQueue = queue;
    cout << "UNIT TEST - Copy Constructor\n";

    if (copiedQueue.numOrders() == queue.numOrders()) {
        cout << "You PASSED the Copy Constructor test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Copy Constructor test.\n";
        return false;
    }
}

bool Tester::testCopyConstructorEdge() {
    MQueue emptyQueue(priorityFn1, MINHEAP, LEFTIST);
    MQueue copiedQueue = emptyQueue;
    cout << "UNIT TEST - Copy Constructor Edge Case\n";

    if (copiedQueue.numOrders() == 0) {
        cout << "You PASSED the Copy Constructor Edge Case test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Copy Constructor Edge Case test.\n";
        return false;
    }
}

bool Tester::testAssignmentOperatorNormal() {
    MQueue queue(priorityFn2, MINHEAP, SKEW);
    MQueue assignedQueue(priorityFn2, MINHEAP, SKEW);
    assignedQueue = queue;
    cout << "UNIT TEST - Assignment Operator Normal Case\n";

    if (assignedQueue.numOrders() == queue.numOrders()) {
        cout << "You PASSED the Assignment Operator Normal Case test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Assignment Operator Normal Case test.\n";
        return false;
    }
}

bool Tester::testAssignmentOperatorEdge() {
    MQueue emptyQueue(priorityFn1, MINHEAP, LEFTIST);
    MQueue assignedQueue(priorityFn2, MAXHEAP, SKEW);
    assignedQueue = emptyQueue;
    cout << "UNIT TEST - Assignment Operator Edge Case\n";

    if (assignedQueue.numOrders() == 0) {
        cout << "You PASSED the Assignment Operator Edge Case test.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the Assignment Operator Edge Case test.\n";
        return false;
    }
}

bool Tester::testEmptyDequeueException() {
    MQueue emptyQueue(priorityFn1, MINHEAP, LEFTIST);
    cout << "UNIT TEST - Dequeue from Empty Queue\n";

    try {
        emptyQueue.getNextOrder();
    } catch (const out_of_range &) {
        cout << "You PASSED the Dequeue from Empty Queue test.\n";
        return true;
    }

    cout << "You DID NOT PASS the Dequeue from Empty Queue test.\n";
    return false;
}

bool Tester::testIncompatibleMerge() {
    MQueue queue1(priorityFn1, MAXHEAP, SKEW);
    MQueue queue2(priorityFn2, MINHEAP, LEFTIST);
    cout << "UNIT TEST - Incompatible Merge\n";

    try {
        queue1.mergeWithQueue(queue2);
    } catch (const domain_error &) {
        cout << "You PASSED the Incompatible Merge test.\n";
        return true;
    }

    cout << "You DID NOT PASS the Incompatible Merge test.\n";
    return false;
}

int main() {
    Tester tester;
    tester.testInsertMinHeap();
    tester.testInsertMaxHeap();
    tester.testRemoveMinHeap();
    tester.testRemoveMaxHeap();
    tester.testLeftistHeapNPL();
    tester.testLeftistHeapProperty();
    tester.testPriorityChange();
    tester.testMergeWithEmptyQueue();
    tester.testCopyConstructor();
    tester.testCopyConstructorEdge();
    tester.testAssignmentOperatorNormal();
    tester.testAssignmentOperatorEdge();
    tester.testEmptyDequeueException();
    tester.testIncompatibleMerge();
    return 0;
}