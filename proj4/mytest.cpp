// CMSC 341 - Fall 2024 - Project 4
#include "filesys.h"
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

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

string namesDB[6] = {"driver.cpp", "test.cpp", "test.h", "info.txt", "mydocument.docx", "tempsheet.xlsx"};

class Tester {
public:
    bool testInsert();               // test inserting files
    bool testFindExisting();         // test finding an existing file
    bool testFindNonExisting();      // test finding a non-existing file
    bool testFindCollisions();       // test finding colliding keys
    bool testRemoveExisting();       // test removing an existing file
    bool testRemoveNonExisting();    // test removing a non-existing file
    bool testRemoveNonColliding();   // test removing non-colliding keys
    bool testRemoveColliding();      // test removing colliding keys without triggering a rehash
    bool testRehashLoadFactor();     // test rehash triggered by load factor
    bool testRehashDeleteRatio();    // test rehash triggered by delete ratio
    bool testIncrementalRehash();    // test incremental rehashing
    bool testRehashPolicyChange();   // test rehash with policy change
};

// test inserting files
bool Tester::testInsert() {
    cout << "UNIT TEST - Insert into Hash Table\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    File file("test.cpp", 123456); // create a test file
    if (fs.insert(file) && (fs.getFile("test.cpp", 123456) == file)) {
        cout << "You PASSED the unit test for inserting files.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for inserting files.\n";
        return false;
    }
}

// test finding an existing file
bool Tester::testFindExisting() {
    cout << "UNIT TEST - Find Existing File\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    File file("test.cpp", 123456); // create a test file
    fs.insert(file); // insert the file
    if (fs.getFile("test.cpp", 123456) == file) {
        cout << "You PASSED the unit test for finding an existing file.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for finding an existing file.\n";
        return false;
    }
}

// test finding a non-existing file
bool Tester::testFindNonExisting() {
    cout << "UNIT TEST - Find Non-Existing File\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    File result = fs.getFile("nonexistent.cpp", 654321); // try to find a non-existing file
    if (result.getName().empty() && result.getDiskBlock() == 0) {
        cout << "You PASSED the unit test for finding a non-existing file.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for finding a non-existing file.\n";
        return false;
    }
}

// test finding colliding keys
bool Tester::testFindCollisions() {
    cout << "UNIT TEST - Find Colliding Keys\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    File file1("test.cpp", 123456); // create first file
    File file2("test.h", 654321);   // create second file
    fs.insert(file1); // insert the first file
    fs.insert(file2); // insert the second file
    if (fs.getFile("test.cpp", 123456) == file1 && fs.getFile("test.h", 654321) == file2) {
        cout << "You PASSED the unit test for finding colliding keys.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for finding colliding keys.\n";
        return false;
    }
}

// test removing an existing file
bool Tester::testRemoveExisting() {
    cout << "UNIT TEST - Remove Existing File\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    File file("test.cpp", 123456); // create a test file
    fs.insert(file); // insert the file

    if (fs.remove(file)) { // try to remove the file
        // validate the file cannot be retrieved
        if (fs.getFile("test.cpp", 123456).getName().empty()) {
            cout << "You PASSED the unit test for removing an existing file.\n";
            return true;
        }
    }
    cout << "You DID NOT PASS the unit test for removing an existing file.\n";
    return false;
}



// test removing a non-existing file
bool Tester::testRemoveNonExisting() {
    cout << "UNIT TEST - Remove Non-Existing File\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    File file("test.cpp", 123456); // create a test file
    if (!fs.remove(file)) { // try to remove a non-existing file
        cout << "You PASSED the unit test for removing a non-existing file.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for removing a non-existing file.\n";
        return false;
    }
}

// test removing a few non-colliding keys
bool Tester::testRemoveNonColliding() {
    cout << "UNIT TEST - Remove Non-Colliding Keys\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system
    File file1("file1.txt", 123456);
    File file2("file2.txt", 123457);
    File file3("file3.txt", 123458);

    // insert the files
    fs.insert(file1);
    fs.insert(file2);
    fs.insert(file3);

    // validate removal of each file
    bool result = fs.remove(file1) && fs.getFile("file1.txt", 123456).getName().empty();
    result = result && fs.remove(file2) && fs.getFile("file2.txt", 123457).getName().empty();
    result = result && fs.remove(file3) && fs.getFile("file3.txt", 123458).getName().empty();

    if (result) {
        cout << "You PASSED the unit test for removing non-colliding keys.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for removing non-colliding keys.\n";
        return false;
    }
}



// test removing colliding keys without triggering a rehash
bool Tester::testRemoveColliding() {
    cout << "UNIT TEST - Remove Colliding Keys Without Rehash\n";
    FileSys fs(MINPRIME, hashCode, QUADRATIC); // create a file system
    File file1("collision1.txt", 123456);
    File file2("collision2.txt", 123457); // collides with file1
    File file3("collision3.txt", 123458); // collides with file1

    // insert colliding files
    fs.insert(file1);
    fs.insert(file2);
    fs.insert(file3);

    // validate removal of each file
    bool result = fs.remove(file1) && fs.getFile("collision1.txt", 123456).getName().empty();
    result = result && fs.remove(file2) && fs.getFile("collision2.txt", 123457).getName().empty();
    result = result && fs.remove(file3) && fs.getFile("collision3.txt", 123458).getName().empty();

    if (result) {
        cout << "You PASSED the unit test for removing colliding keys without rehash.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for removing colliding keys without rehash.\n";
        return false;
    }
}



// test rehash triggered by load factor
bool Tester::testRehashLoadFactor() {
    cout << "UNIT TEST - Rehash by Load Factor\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    for (int i = 0; i < 60; ++i) { // insert enough files to trigger rehash
        fs.insert(File("file" + to_string(i), DISKMIN + i));
    }
    if (fs.lambda() <= 0.5) { // check that load factor is within limits
        cout << "You PASSED the unit test for rehashing by load factor.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for rehashing by load factor.\n";
        return false;
    }
}

// test rehash triggered by delete ratio
bool Tester::testRehashDeleteRatio() {
    cout << "UNIT TEST - Rehash by Delete Ratio\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    for (int i = 0; i < 20; ++i) { // insert files
        fs.insert(File("file" + to_string(i), DISKMIN + i));
    }
    for (int i = 0; i < 18; ++i) { // remove most files to trigger rehash
        fs.remove(File("file" + to_string(i), DISKMIN + i));
    }
    if (fs.deletedRatio() <= 0.8) { // check deleted ratio is within limits
        cout << "You PASSED the unit test for rehashing by delete ratio.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for rehashing by delete ratio.\n";
        return false;
    }
}

// test incremental rehashing
bool Tester::testIncrementalRehash() {
    cout << "UNIT TEST - Incremental Rehashing\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    for (int i = 0; i < 50; ++i) { // insert files to trigger rehash
        fs.insert(File("file" + to_string(i), DISKMIN + i));
    }
    for (int i = 0; i < 12; ++i) { // perform incremental rehashing
        fs.insert(File("newfile" + to_string(i), DISKMAX - i));
    }
    if (fs.lambda() <= 0.5) { // check rehashing is successful
        cout << "You PASSED the unit test for incremental rehashing.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for incremental rehashing.\n";
        return false;
    }
}

// test rehash with collision policy change
bool Tester::testRehashPolicyChange() {
    cout << "UNIT TEST - Rehash with Policy Change\n";
    FileSys fs(MINPRIME, hashCode, LINEAR); // create a file system with linear probing
    fs.changeProbPolicy(DOUBLEHASH); // request collision policy change
    for (int i = 0; i < 50; ++i) { // insert files to trigger rehash
        fs.insert(File("file" + to_string(i), DISKMIN + i));
    }
    if (fs.lambda() <= 0.5) { // check rehashing with new policy is successful
        cout << "You PASSED the unit test for rehashing with policy change.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for rehashing with policy change.\n";
        return false;
    }
}

int main() {
    Tester tester;
    tester.testInsert();
    tester.testFindExisting();
    tester.testFindNonExisting();
    tester.testFindCollisions();
    tester.testRemoveExisting();
    tester.testRemoveNonExisting();
    tester.testRemoveNonColliding();
    tester.testRemoveColliding();
    tester.testRehashLoadFactor();
    tester.testRehashDeleteRatio();
    tester.testIncrementalRehash();
    tester.testRehashPolicyChange();
    return 0;
}