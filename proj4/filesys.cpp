// CMSC 341 - Fall 2024 - Project 4 
#include "filesys.h"

// constructor for creating a hash table with a given size and hash function
FileSys::FileSys(int size, hash_fn hash, prob_t probing) {
    m_hash = hash; // set the hash function
    m_currProbing = probing; // set the probing policy
    m_newPolicy = DEFPOLCY; // default policy

    // ensure size is a prime within bounds
    if (size < MINPRIME) size = MINPRIME;
    else if (size > MAXPRIME) size = MAXPRIME;
    else if (!isPrime(size)) size = findNextPrime(size);

    m_currentCap = size; // set capacity of the hash table
    m_currentTable = new File*[m_currentCap](); // create hash table
    m_currentSize = 0; // start with no files
    m_currNumDeleted = 0; // and no deleted files

    m_oldTable = nullptr; // no old table to start with
}

// destructor
FileSys::~FileSys() {
    // delete current table
    if (m_currentTable) {
        for (int i = 0; i < m_currentCap; i++) {
            delete m_currentTable[i]; // delete individual files
        }
        delete[] m_currentTable; // delete the table array
    }

    // delete old table
    if (m_oldTable) {
        for (int i = 0; i < m_oldCap; i++) {
            delete m_oldTable[i]; // delete individual files
        }
        delete[] m_oldTable; // delete the table array
    }
}


// changes the probing policy to be used during the next rehash
void FileSys::changeProbPolicy(prob_t policy) {
    m_newPolicy = policy; // store the new probing policy
}

// inserts a file into the hash table
bool FileSys::insert(File file) {
    // validate the file block number
    if (file.getDiskBlock() < DISKMIN || file.getDiskBlock() > DISKMAX) {
        return false; // invalid block number
    }

    // find the correct index using the helper function
    int index = findIndex(file, m_currentCap);

    // check if the file already exists at the index
    if (m_currentTable[index] != nullptr && *m_currentTable[index] == file) {
        return false; // no duplicates allowed
    }

    // insert the new file or update the existing bucket
    if (m_currentTable[index] == nullptr) {
        m_currentTable[index] = new File(file); // create a new file at the index
    } else {
        *m_currentTable[index] = file; // update the file at the index
    }
    m_currentTable[index]->setUsed(true); // mark the bucket as used
    m_currentSize++; // increment the number of elements in the table

     
    rehashCheck(); // rehash if necessary
    return true; // file inserted successfully
}

// removes a file from the hash table
bool FileSys::remove(File file) {
    int index = findIndex(file, m_currentCap); // locate file in the current table

    if (index != -1 && m_currentTable[index] != nullptr && m_currentTable[index]->getUsed()) {
        m_currentTable[index]->setUsed(false); // mark the slot as deleted
        m_currNumDeleted++; // increment the deleted count
        m_currentSize--; // decrement the size of live data
        return true; // successfully removed
    }

    // Check in the old table if rehashing is in progress
    if (m_oldTable != nullptr) {
        index = findIndex(file, m_oldCap);
        if (index != -1 && m_oldTable[index] != nullptr && m_oldTable[index]->getUsed()) {
            m_oldTable[index]->setUsed(false); // mark the slot as deleted
            m_oldSize--; // decrement size in the old table
            return true; // successfully removed from old table
        }
    }

    return false; // file not found
}



// retrieves a file from the hash table using the file's name and block number
const File FileSys::getFile(string name, int block) const {
    int index = m_hash(name) % m_currentCap; // initial index
    int startIndex = index; // to avoid infinite loops
    int probeCount = 0;

    // Search in the current table
    do {
        if (m_currentTable[index] != nullptr && m_currentTable[index]->getUsed() &&
            m_currentTable[index]->getName() == name && m_currentTable[index]->getDiskBlock() == block) {
            return *m_currentTable[index]; // found in current table
        }
        probeCount++;
        if (m_currProbing == LINEAR) {
            index = (index + 1) % m_currentCap;
        } else if (m_currProbing == QUADRATIC) {
            index = (index + probeCount * probeCount) % m_currentCap;
        } else if (m_currProbing == DOUBLEHASH) {
            int hash2 = 11 - (m_hash(name) % 11);
            index = (index + probeCount * hash2) % m_currentCap;
        }
    } while (index != startIndex);

    // Search in the old table if rehashing is in progress
    if (m_oldTable != nullptr) {
        index = m_hash(name) % m_oldCap;
        startIndex = index;
        probeCount = 0;

        do {
            if (m_oldTable[index] != nullptr && m_oldTable[index]->getUsed() &&
                m_oldTable[index]->getName() == name && m_oldTable[index]->getDiskBlock() == block) {
                return *m_oldTable[index]; // found in old table
            }
            probeCount++;
            if (m_oldProbing == LINEAR) {
                index = (index + 1) % m_oldCap;
            } else if (m_oldProbing == QUADRATIC) {
                index = (index + probeCount * probeCount) % m_oldCap;
            } else if (m_oldProbing == DOUBLEHASH) {
                int hash2 = 11 - (m_hash(name) % 11);
                index = (index + probeCount * hash2) % m_oldCap;
            }
        } while (index != startIndex);
    }

    return File(); // not found
}


// updates the disk block number for a file within the specified range
bool FileSys::updateDiskBlock(File file, int block) {
    if (block < DISKMIN || block > DISKMAX) return false; // ensure block number is valid

    int index = findIndex(file, m_currentCap); // find the file in the table
    if (index != -1 && m_currentTable[index] != nullptr && m_currentTable[index]->getUsed()) {
        m_currentTable[index]->setDiskBlock(block); // update the block number
        return true; // successfully updated
    }
    return false; // file not found
}

// calculates the load factor of the hash table
float FileSys::lambda() const {
    return static_cast<float>(m_currentSize) / m_currentCap; // calculate load factor
}

// calculates the ratio of deleted buckets to total occupied buckets
float FileSys::deletedRatio() const {
    return m_currentSize + m_currNumDeleted > 0
               ? static_cast<float>(m_currNumDeleted) / (m_currentSize + m_currNumDeleted)
               : 0.0f; // return 0 if no occupied buckets
}

// helper function to rehash incrementally
void FileSys::rehash() {
    if (!m_oldTable) { // initialize rehashing
        int newCap = findNextPrime(4 * (m_currentSize - m_currNumDeleted)); // calculate new capacity
        m_oldTable = m_currentTable; // mark current table as old table
        m_oldCap = m_currentCap; // store old table capacity
        m_transferIndex = 0; // start transfer from the beginning of the old table
        m_oldNumDeleted = m_currNumDeleted;


        m_currentCap = newCap; // update new table capacity
        m_currentTable = new File*[m_currentCap](); // allocate new table
        m_currentSize = 0; // reset size for new table
        m_currNumDeleted = 0; // reset deleted count
    }

   // transfer 25% of the old table to the new table
int transferLimit = m_oldCap / 4; // process 25% of the old table
for (int i = 0; i < transferLimit && m_transferIndex < m_oldCap; i++, m_transferIndex++) {
    if (m_oldTable[m_transferIndex] && m_oldTable[m_transferIndex]->getUsed()) {
        File& currentFile = *m_oldTable[m_transferIndex];

        // insert into the new table using the correct probing policy
        int newIndex = m_hash(currentFile.getName()) % m_currentCap;
        int probeCount = 0;

        while (m_currentTable[newIndex] != nullptr) {
            probeCount++;
            if (m_currProbing == LINEAR) {
                newIndex = (newIndex + 1) % m_currentCap;
            } else if (m_currProbing == QUADRATIC) {
                newIndex = (newIndex + probeCount * probeCount) % m_currentCap;
            } else if (m_currProbing == DOUBLEHASH) {
                int hash2 = 11 - (m_hash(currentFile.getName()) % 11);
                newIndex = (newIndex + probeCount * hash2) % m_currentCap;
            }
        }

        m_currentTable[newIndex] = m_oldTable[m_transferIndex]; // transfer file
        m_currentSize++; // increment new table size
        m_oldTable[m_transferIndex] = nullptr; // clear old slot to avoid dangling pointers
    }
}


    // finalize rehashing if all data is transferred
    if (m_transferIndex >= m_oldCap) {
        delete[] m_oldTable; // free old table memory
        m_oldTable = nullptr; // reset old table pointer
        m_oldCap = 0; // reset old table capacity
        m_transferIndex = 0; // reset transfer index
    }
}


// helper to check and trigger rehash if needed
void FileSys::rehashCheck() {
    // check if rehashing criteria are met
    if ((lambda() > 0.5 || deletedRatio() > 0.8) && !m_oldTable) { // only trigger rehash if no ongoing rehash
        m_oldTable = m_currentTable; // move current table to old table
        m_oldCap = m_currentCap;
        m_transferIndex = 0; // reset transfer index

        // create a new table with the correct capacity
        m_currentCap = findNextPrime((m_currentSize - m_currNumDeleted) * 4);
        m_currentTable = new File*[m_currentCap]();
        m_currentSize = 0;
        m_currNumDeleted = 0;
        m_currProbing = m_newPolicy; // apply new policy if requested
    }

    // continue incremental rehashing
    if (m_oldTable) {
        rehash();
    }
}


// helper function to find the correct index for a file
int FileSys::findIndex(const File& file, int tableSize) const {
    int index = m_hash(file.getName()) % tableSize; // calculate initial index
    int probeCount = 0; // track probing attempts

    while (probeCount < tableSize) { // ensure we don't exceed the table size
        // check if slot is empty or matches the file
        if (m_currentTable[index] == nullptr || 
            (!m_currentTable[index]->getUsed() && *m_currentTable[index] == file)) {
            return index; // valid index for insertion or update
        }

        // collision resolution using the specified probing policy
        probeCount++;
        if (m_currProbing == LINEAR) {
            index = (index + 1) % tableSize; // linear probing
        } else if (m_currProbing == QUADRATIC) {
            index = (index + probeCount * probeCount) % tableSize; // quadratic probing
        } else if (m_currProbing == DOUBLEHASH) {
            int hash2 = 11 - (m_hash(file.getName()) % 11); // secondary hash
            index = (index + probeCount * hash2) % tableSize; // double hashing
        }
    }

    return -1; // table is full or no valid index found
}



void FileSys::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool FileSys::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int FileSys::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
