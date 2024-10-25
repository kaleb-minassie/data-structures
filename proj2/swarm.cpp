// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"

// default constructor
Swarm::Swarm() {
    m_root = nullptr;
    m_type = NONE;
}

// constructor to create an empty tree and set the tree type
Swarm::Swarm(TREETYPE type) {
    m_root = nullptr;
    m_type = type; // set the tree type (bst, avl, or splay)
}

// destructor
Swarm::~Swarm() {
    clear();
}

// recursively clears the tree by deallocating all nodes
void Swarm::clear() {
    clearHelper(m_root); // recursively clears all nodes
    m_root = nullptr;    // resets the tree by removing all nodes
}

// helper function to recursively clear nodes
void Swarm::clearHelper(Robot* node) {
    if (node != nullptr) { // if node is not null
        clearHelper(node->m_left); // clear left subtree
        clearHelper(node->m_right); // clear right subtree
        delete node; // delete current node
    }
}

// inserts a new robot into the tree in the correct position
void Swarm::insert(const Robot& robot) {
    Robot* newNode = new Robot(robot); // create new node with given robot
    m_root = insertHelper(m_root, newNode); // insert robot into the tree
    if (m_type == SPLAY) { // if tree type is splay
        m_root = splay(m_root, robot.m_id); // splay the inserted node to root
    }
}

// helper function to recursively insert a robot into the correct position
Robot* Swarm::insertHelper(Robot* node, Robot* newNode) {
    if (node == nullptr) { // if current node is null
        return newNode; // return new node
    }
    if (newNode->m_id < node->m_id) { // if robot id is less than current node id
        node->m_left = insertHelper(node->m_left, newNode); // insert into left subtree
    } else if (newNode->m_id > node->m_id) { // if robot id is greater than current node id
        node->m_right = insertHelper(node->m_right, newNode); // insert into right subtree
    }
    if (m_type == AVL) { // if tree type is avl
        updateHeight(node); // update height of current node
        return balanceAVL(node); // balance the tree
    }
    return node; // return current node
}

// helper function to find the minimum node
Robot* Swarm::findMin(Robot* node) {
    while (node->m_left != nullptr) { // traverse left until null
        node = node->m_left; // move to left child
    }
    return node; // return minimum node
}

// helper function to find the maximum node
Robot* Swarm::findMax(Robot* node) {
    while (node->m_right != nullptr) { // traverse right until null
        node = node->m_right; // move to right child
    }
    return node; // return maximum node
}

// function to remove a robot from the tree
void Swarm::remove(int id) {
    if (m_type == SPLAY) { // if tree type is splay
        m_root = splay(m_root, id); // splay the node to be removed to root
    } else {
        m_root = removeHelper(m_root, id); // remove node with given id
    }
}

// helper function to recursively find and remove a robot by id
Robot* Swarm::removeHelper(Robot* node, int id) {
    if (node == nullptr) return nullptr; // if node is null, return null
    if (id < node->m_id) { // if id is less than current node id
        node->m_left = removeHelper(node->m_left, id); // remove from left subtree
    } else if (id > node->m_id) { // if id is greater than current node id
        node->m_right = removeHelper(node->m_right, id); // remove from right subtree
    } else { // if id matches current node id
        if (node->m_left == nullptr) { // if left child is null
            Robot* rightChild = node->m_right; // store right child
            delete node; // delete current node
            return rightChild; // return right child
        } else if (node->m_right == nullptr) { // if right child is null
            Robot* leftChild = node->m_left; // store left child
            delete node; // delete current node
            return leftChild; // return left child
        } else { // if both children are present
            Robot* minNode = findMin(node->m_right); // find minimum node in right subtree
            node->m_id = minNode->m_id; // copy minimum node id to current node
            node->m_right = removeHelper(node->m_right, minNode->m_id); // remove minimum node from right subtree
        }
    }
    if (m_type == AVL) { // if tree type is avl
        updateHeight(node); // update height of current node
        return balanceAVL(node); // balance the tree
    }
    return node; // return current node
}

// helper function to do a left rotation
Robot* Swarm::rotateLeft(Robot* x) {
    Robot* y = x->m_right;                // y will be the new root
    Robot* tempSubtree = y->m_left;       // store y's left child for later

    // perform the rotation
    y->m_left = x;                        // x becomes the left child of y
    x->m_right = tempSubtree;             // reattach the stored subtree

    // update heights
    updateHeight(x);
    updateHeight(y);

    // return the new root
    return y;
}

// helper function to perform a right rotation
Robot* Swarm::rotateRight(Robot* y) {
    Robot* x = y->m_left; // set x to left child of y
    y->m_left = x->m_right; // set left child of y to right child of x
    x->m_right = y; // set right child of x to y
    updateHeight(y); // update height of y
    updateHeight(x); // update height of x
    return x; // return new root
}

// helper function to splay the node with the given id to the root
Robot* Swarm::splay(Robot* root, int id) {
    // base case: if the root is null or the node is already at the root
    if (root == nullptr || root->m_id == id) {
        return root; // splay not needed
    }

    // case 1: node is in the left subtree
    if (id < root->m_id) {
        // node isn't in the tree
        if (root->m_left == nullptr) {
            return root; // splay not needed
        }

        // zig-zig case (left-left)
        if (id < root->m_left->m_id) {
            root->m_left->m_left = splay(root->m_left->m_left, id); // go left-left
            root = rotateRight(root); // first right rotation (zig-zig)
        }
        // zig-zag case (left-right)
        else if (id > root->m_left->m_id) {
            root->m_left->m_right = splay(root->m_left->m_right, id); // go left-right
            if (root->m_left->m_right != nullptr) {
                root->m_left = rotateLeft(root->m_left); // first left rotation (zig-zag)
            }
        }

        // zig case: single right rotation
        if (root->m_left == nullptr) {
            return root; // no left child, return root
        } else {
            return rotateRight(root); // single right rotation (zig)
        }
    }

    // case 2: node is in the right subtree
    else {
        // node isn't in the tree
        if (root->m_right == nullptr) {
            return root; // splay not needed
        }

        // zig-zig case (right-right)
        if (id > root->m_right->m_id) {
            root->m_right->m_right = splay(root->m_right->m_right, id); // go right-right
            root = rotateLeft(root); // first left rotation (zig-zig)
        }
        // zig-zag case (right-left)
        else if (id < root->m_right->m_id) {
            root->m_right->m_left = splay(root->m_right->m_left, id); // go right-left
            if (root->m_right->m_left != nullptr) {
                root->m_right = rotateRight(root->m_right); // first right rotation (zig-zag)
            }
        }

        // zig case: single left rotation
        if (root->m_right == nullptr) {
            return root; // no right child, return root
        } else {
            return rotateLeft(root); // single left rotation (zig)
        }
    }
}

// overloaded assignment operator (deep copy)
const Swarm& Swarm::operator=(const Swarm& rhs) {
    if (this != &rhs) { // avoid self-assignment
        clear(); // clear current object to avoid memory leaks

        // check if the right-hand side tree is empty
        if (rhs.m_root == nullptr) {
            m_root = nullptr;
            m_type = rhs.m_type;
            return *this;
        }

        // perform a recursive deep copy of the tree
        m_root = copyTree(rhs.m_root); // make a deep copy of the rhs root
        m_type = rhs.m_type;
    }
    return *this; // return the current tree
}

// helper function to copy the entire tree recursively
Robot* Swarm::copyTree(Robot* rhsNode) {
    if (rhsNode == nullptr) {
        return nullptr;
    }

    // creating a new node with the same values as rhsNode
    Robot* newNode = new Robot(rhsNode->m_id, rhsNode->m_type, rhsNode->m_state);
    newNode->m_height = rhsNode->m_height;

    // recursively copy the left and right subtrees
    newNode->m_left = copyTree(rhsNode->m_left);
    newNode->m_right = copyTree(rhsNode->m_right);

    return newNode;
}

// getter
// returns the current tree type (bst, avl, or splay)
TREETYPE Swarm::getType() const {
    return m_type;
}

// helper function to return the height of a node (if node is nullptr, height is -1)
int Swarm::getHeight(Robot* node) const {
    if (node == nullptr) {
        return -1; // return -1 if the node is null
    } else {
        return node->m_height; // otherwise, return the node height
    }
}

// helper function to update height using the max of the children's heights
void Swarm::updateHeight(Robot* node) {
    if (node != nullptr) {
        int leftHeight = (node->m_left != nullptr) ? node->m_left->m_height : -1;
        int rightHeight = (node->m_right != nullptr) ? node->m_right->m_height : -1;
        node->m_height = 1 + max(leftHeight, rightHeight);
    }
}

// helper function to balance the avl at given node
Robot* Swarm::balanceAVL(Robot* node) {
    if (node == nullptr) return nullptr; // if node is null, no balancing needed

    updateHeight(node); // update height of the current node

    // calculate the balance factor
    int balance = getHeight(node->m_left) - getHeight(node->m_right);

    // left-left case
    if (balance > 1 && getHeight(node->m_left->m_left) >= getHeight(node->m_left->m_right)) {
        return rotateRight(node);
    }
    // left-right case
    if (balance > 1 && getHeight(node->m_left->m_left) < getHeight(node->m_left->m_right)) {
        node->m_left = rotateLeft(node->m_left);
        return rotateRight(node);
    }
    // right-right case
    if (balance < -1 && getHeight(node->m_right->m_right) >= getHeight(node->m_right->m_left)) {
        return rotateLeft(node);
    }
    // right-left case
    if (balance < -1 && getHeight(node->m_right->m_right) < getHeight(node->m_right->m_left)) {
        node->m_right = rotateRight(node->m_right);
        return rotateLeft(node);
    }

    // return the balanced node
    return node;
}

// setter
// sets the tree type and adjust the tree if necessary
void Swarm::setType(TREETYPE type) {
    if (m_type == AVL && type != AVL) {
         // if changing from avl to non-avl
        m_type = type; // set new type
    } else if (type == AVL) {
        Robot* copy = m_root;
        m_root=nullptr;
        // setting type to avl
        m_type=AVL;
        reconstructAVL(copy); // reconstruct tree as avl
    } else { // if changing to any other type
        m_type = type; // set new type
    }
}


// helper function to reconstruct the current tree into an avl tree
void Swarm::reconstructAVL(Robot* copy) {
// check if copy is not null before proceeding    
        if (copy != nullptr) {
        // perform a postorder traversal on the left subtree
        reconstructAVL(copy->m_left); 
        // perform a postorder traversal on the right subtree
        reconstructAVL(copy->m_right);
        // reset left child pointer
        copy->m_left = nullptr;
        // reset right child pointer
        copy->m_right= nullptr;
        // reset height to zero
        copy->m_height=0;
        // insert the current node into the new AVL tree
        m_root=insertHelper(m_root,copy);
    }

}

// function to dump the tree structure
void Swarm::dumpTree() const {
    dump(m_root);
}

// helper function to perform an in-order traversal and print tree structure
void Swarm::dump(Robot* aBot) const {
    if (aBot != nullptr) {
        cout << "(";
        dump(aBot->m_left); // first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height; // second visit the node itself
        dump(aBot->m_right); // third visit the right child
        cout << ")";
    }
}
