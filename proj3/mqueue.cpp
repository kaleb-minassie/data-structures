// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"

// constructor to initialize an empty queue
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{
   m_heap = nullptr;       // set root to null
    m_size = 0;             // set initial size to zero
    m_priorFunc = priFn;    // store the priority function
    m_heapType = heapType;  // set heap type (min or max)
    m_structure = structure; // set structure type (skew or leftist)
}

// destructor
MQueue::~MQueue(){
    clear();
}

// clears the heap by deallocating all nodes and resetting the root
void MQueue::clear() {
    clearTree(m_heap); // recursively clear all nodes
    m_heap = nullptr;  // reset the heap root to null
    m_size = 0;        // reset the size to zero
}

// helper function to recursively clear all nodes in the heap
void MQueue::clearTree(Node* node) {
    if (node != nullptr) {           // if node exists
        clearTree(node->m_left);     // recursively clear the left subtree
        clearTree(node->m_right);    // recursively clear the right subtree
        delete node;                 // delete the current node
    }
}


// copy constructor
MQueue::MQueue(const MQueue& rhs)
{
    m_heap = nullptr;
    *this = rhs; // using assignment operator for deep copy
}

// assignment operator to create a deep copy of the queue
MQueue& MQueue::operator=(const MQueue& rhs) {
     if (this != &rhs) { // check for self-assignment
        clear(); // clear the current heap
        m_heap = copyTree(rhs.m_heap); // recursive deep copy of the tree
        m_size = rhs.m_size;           // copy size
        m_priorFunc = rhs.m_priorFunc; // copy priority function
        m_heapType = rhs.m_heapType;   // copy heap type
        m_structure = rhs.m_structure; // copy structure type
    }

    return *this;   // return the current heap

}

// helper to create a deep copy of the heap
Node* MQueue::copyTree(Node* root) {
    if (root == nullptr) return nullptr;  // base case: if root is null, return null
    
    Node* newNode = new Node(root->m_order); // create a new node with the same order data
    newNode->m_left = copyTree(root->m_left); // recursively copy the left subtree
    newNode->m_right = copyTree(root->m_right); // recursively copy the right subtree
    
    // set NPL value if the heap structure is leftist
    if (m_structure == LEFTIST) {
        newNode->setNPL(root->getNPL()); // copy the NPL if it's a leftist heap
    }
    
    return newNode; // return the copied node
}

// merges the current queue with another queue, leaves rhs empty
void MQueue::mergeWithQueue(MQueue& rhs) {
   // check for self-merging
    if (this == &rhs) {
        throw domain_error("Cannot merge with itself");
    }

    // check if priority function and structure type match
    if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure) {
        throw domain_error("Incompatible queues"); // check for compatibility
    }
    // merge both heaps and update size
    m_heap = merge(m_heap, rhs.m_heap);
    m_size += rhs.m_size;

    rhs.m_heap = nullptr; // clear rhs heap
    rhs.m_size = 0;       // reset size of rhs
}


// helper function to merge two heaps based on the priority and structure type
Node* MQueue::merge(Node* p1, Node* p2) {
    if (p1 == nullptr) return p2; // if p1 is null, return p2
    if (p2 == nullptr) return p1; // if p2 is null, return p1
    
    // check priority based on min-heap or max-heap
    if ((m_heapType == MINHEAP && m_priorFunc(p1->m_order) > m_priorFunc(p2->m_order)) ||
        (m_heapType == MAXHEAP && m_priorFunc(p1->m_order) < m_priorFunc(p2->m_order))) {
        // switch p1 and p2 if p2 has higher priority than p1
        Node* temp = p1;
        p1 = p2;
        p2 = temp;
    }

    // for skew heaps, swap the left and right children of p1
    if (m_structure == SKEW) {
        Node* temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
    }

    // recursively merge p2 into the left child of p1
    p1->m_left = merge(p1->m_left, p2);

    // keep leftist heap balanced by adjusting NPL values
    if (m_structure == LEFTIST) {
        if (p1->m_right == nullptr || 
           (p1->m_left != nullptr && p1->m_left->getNPL() < p1->m_right->getNPL())) {
            // swap left and right children if left NPL is smaller
            Node* temp = p1->m_left;
            p1->m_left = p1->m_right;
            p1->m_right = temp;
        }
        p1->setNPL((p1->m_right ? p1->m_right->getNPL() : -1) + 1); // update NPL
    }

    return p1; // return merged heap
}


// inserts an order into the queue, maintaining heap property
bool MQueue::insertOrder(const Order& order) {
    int priority = m_priorFunc(order); // compute priority
    if (priority == 0) return false;   // invalid priority, do not insert

    Node* newNode = new Node(order);   // create new node for order
    m_heap = merge(m_heap, newNode);   // merge new node with existing heap
    m_size++;                          // increment size
    return true;                       // insertion successful
}

// returns the current number of orders in the queue
int MQueue::numOrders() const {
    return m_size;
}

// returns the current priority function
prifn_t MQueue::getPriorityFn() const {
    return m_priorFunc;
}

// removes and returns the highest priority order from the queue
Order MQueue::getNextOrder() {
    if (m_heap == nullptr) throw out_of_range("Heap is empty"); // throw error if heap is empty
    
    Order topOrder = m_heap->m_order; // save the highest priority order (at root)
    Node* oldRoot = m_heap;           // keep track of current root

    // merge root's left and right children to form new heap
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    delete oldRoot;                   // remove the old root
    m_size--;                         // reduce the size of the heap
    return topOrder;                  // return the saved order
}

// helper to rebuild the heap with current nodes and new priority function
void MQueue::rebuildHeap() {
    if (m_heap == nullptr) return; // exit if heap is already empty

    Node* oldHeap = m_heap; // keep the current heap to reprocess
    m_heap = nullptr;       // reset heap root to empty
    m_size = 0;             // reset heap size

    // go through all nodes and reinsert them into the new heap
    while (oldHeap != nullptr) {
        insertOrder(oldHeap->m_order);             // add the node's order to the new heap
        Node* next = merge(oldHeap->m_left, oldHeap->m_right); // merge left and right to get next node
        delete oldHeap;                            // delete old node
        oldHeap = next;                            // move to the next node
    }
}

// sets a new priority function and heap type, then rebuilds the heap
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_priorFunc = priFn;     // update the priority function
    m_heapType = heapType;   // update the heap type 
    rebuildHeap();           // rebuild the heap with new priority function
}

// sets a new structure type (skew or leftist) and rebuilds the heap
void MQueue::setStructure(STRUCTURE structure) {
    m_structure = structure; // update structure type
    rebuildHeap();           // rebuild the heap with the new structure
}

// returns the current structure type
STRUCTURE MQueue::getStructure() const {
    return m_structure;
}

// returns the current heap type
HEAPTYPE MQueue::getHeapType() const {
    return m_heapType;
}

// prints the orders in preorder traversal format
void MQueue::printOrderQueue() const {
    printPreorder(m_heap); // start preorder traversal from root
}

// helper function to print all nodes in preorder
void MQueue::printPreorder(Node* node) const {
    if (node == nullptr) return;             // do nothing if the node is null
    cout << node->m_order << "\n";           // print the order at this node
    printPreorder(node->m_left);             // print left subtree
    printPreorder(node->m_right);            // print right subtree
}

void MQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    else
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout  << "Customer: " << order.getCustomer()
        << ", importance: " << order.getImportance() 
        << ", quantity: " << order.getQuantity() 
        << ", order placement time: " << order.getFIFO();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_order;
  return sout;
}
