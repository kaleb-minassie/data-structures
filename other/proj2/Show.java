
enum STATE {
    LIGHTON, LIGHTOFF
};

enum LIGHTCOLOR {
    RED, GREEN, BLUE
};

public class Show {
    public int DEFAULT_HEIGHT = 0;
    public int DEFAULT_ID = 0;
    public LIGHTCOLOR DEFAULT_LIGHT = LIGHTCOLOR.RED;
    public STATE DEFAULT_STATE = STATE.LIGHTON;
    public int MINID = 10000;
    public int MAXID = 99999;

    /**
     * The constructor performs the required initializations.
     */
    public Show() {
        m_root = null;
    }

    /**
     * Copy constructor. It creates an exact deep copy of the rhs.
     * 
     * @param rhs
     */
    
    
     public Show(Show rhs) {
        if (rhs.m_root != null) {
            m_root = deepCopy(rhs.m_root);
        } else {
            m_root = null;
        }
    }
    
    /**
     * Returns the root of the BST
     * (needed for testing)
     * 
     * @return a reference to the Drone at the root
     */
    public Drone getRoot() {
        return m_root;
    }

    /**
     * This function inserts a Drone object into the tree
     * in the proper position. The Drone’s m_id should
     * be used as the key to traverse the Show tree and
     * abide by BST traversal rules. The comparison
     * operators (>, <, ==, !=) work with the int type in Java.
     * A Drone id is a unique number in the range MINID - MAXID.
     * We do not allow a duplicate id or an object with
     * invalid id in the tree.
     * Note:In the Show tree data structure every node
     * is a Drone object which is represented as a reference
     * to the Drone object. Therefore, for every insertion we
     * need to allocate memory and use the information of aDrone
     * to initialize the new node. Memory allocation takes place
     * in the Show class.
     * After an insertion, we should also update the height of
     * each node on the path traversed down the tree as well as
     * check for an imbalance at each node in this path.
     * 
     * @param aDrone - reference to a Drone object
     */
    public void insert(Drone aDrone) {
        //Inserting a new drone into the AVL tree
          // Check if the ID of aDrone is within the valid range
    if (aDrone.getID() < MINID || aDrone.getID() > MAXID) {
        // Handle invalid ID here (e.g., throw an exception or return)
        return;
    }
    
    
    // Check if the drone is already in the tree
    if (findDrone(aDrone.getID())) {
        // Handle the case where the drone with the same ID already exists
        // You can choose to update or skip this insertion
        return;
    }
    
        m_root = insertNode(m_root, aDrone);
    }
    /**
     * The clear function makes it an empty tree.
     */
    public void clear() {
        m_root = null;
    }

    /**
     * The remove function traverses the tree to find a node with
     * the id and removes it from the tree.
     * After an removal, we should also update the height of
     * each node on the path traversed down the tree as well as
     * check for an imbalance at each node in this path.
     * 
     * @param id the id of the Drone that is to be removed
     */
    public void remove(int id) {
        // Removing a drone with a specified ID from the AVL tree
        // base case
        if (m_root == null) {
            return;
        }
        if (id < MINID || id > MAXID) {
            return;
        }
        // if drone is not in tree, return
        if (!findDrone(id)) {
            return;
        }
        m_root = removeNode(m_root, id);
    }
    /**
     * This function returns a list of all drones in the tree
     * in the ascending order of IDs. The information for every
     * Drone object will be placed in a new line. For the format
     * of output please refer to the myShowTest for sample output
     * 
     * @return string of the list of all drones in the tree in
     *         ascending order
     */
    public String listDrones() {
        return listDronesInOrder(m_root);
    }



    /**
     * This function finds the node with id in the tree and sets
     * its m_state member variable to state. If the operation is
     * successful, the function returns true otherwise it returns
     * false. For example, when the drone with id does not exist in
     * the tree the function returns false.
     * 
     * @param id    id of the Drone to change the state of
     * @param state the state that Drones state will be set to
     * @return true if successful, false if not successful
     */
    public boolean setState(int id, STATE state) {
        return setDroneState(m_root, id, state);
    }
    

    /**
     * This function traverses the tree, finds all drones with LIGHTOFF
     * state and removes them from the tree. The final tree must be a
     * balanced AVL tree.
     */
    public void removeLightOff() {
        m_root = removeLightOffDrones(m_root);
    }
   
    /**
     * This function traverses the tree, finds all drones with the
     * color specified by aColor variable and returns the total
     * number of drones of that color in the show.
     * 
     * @param aColor color of Drones to count
     * @return the number of Drones that are set to this color
     */
    public int countDrones(LIGHTCOLOR aColor) {
        return countDronesByColor(m_root, aColor);
    }
    
    /**
     * This function returns true if it finds the node with
     * id in the tree, otherwise it returns false.
     * 
     * @param id id of the Drone searching for
     * @return true if Drone found, false otherwise
     */
    public boolean findDrone(int id) {
        return findDroneByID(m_root, id);
    }
    
    /**
     * Provided for you to check the balance of your tree.
     * You can place the result of this method into the tree
     * viewer tool mentioned in the description of this project
     * 
     * @return a string that represents the structures of the tree
     */
    public String dumpTree() {
        String result = "";
        result = dump(m_root, result);
        return result;
    }


    /**
     * Helper function for the dumpTree
     * 
     * @param aDrone current Drone being visited
     * @param result current string
     * @return string showing the current structure of the tree
     */
    public String dump(Drone aDrone, String result) {
        if (aDrone == null) {
            return result;
        }
        result += "(";
        result = dump(aDrone.m_left, result);// first visit the left child
        result += aDrone.m_id + ":" + aDrone.m_height;// second visit the node itself
        result = dump(aDrone.m_right, result);// third visit the right child
        result += ")";
        return result;
    }

    private Drone m_root;// the root of the BST

    private Drone deepCopy(Drone root) {
        if (root == null) {
            return null;
        }
    
        // Create a new Drone with the same ID, type, and state
        Drone copy = new Drone(root.m_id, root.m_type, root.m_state);
    
        // Recursively copy the left and right subtrees
        copy.m_left = deepCopy(root.m_left);
        copy.m_right = deepCopy(root.m_right);
    
        // Copy the height
        copy.m_height = root.m_height;
    
        return copy;
    }
    
    
    

    private int height (Drone node) {
        if (node == null) {
            return -1;
        }
        return node.m_height;
    }

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
  // Using recursive helper method to insert a new drone into the AVL tree rooted at the given node
  private Drone insertNode(Drone root, Drone aDrone) {
    if (root == null) {
        return aDrone;
    }

    // Insert the node as in a standard binary search tree
    if (aDrone.getID() < root.getID()) {
        root.setLeft(insertNode(root.getLeft(), aDrone));
    } else if (aDrone.getID() > root.getID()) {
        root.setRight(insertNode(root.getRight(), aDrone));
    } else {
        // Handle duplicates if needed
        // You can decide what to do if two nodes have the same ID
        // For example, you can keep a count of duplicates in the node
        // or skip insertion.
    }

    // Update the height of the current node
    root.setHeight(1 + Math.max(height(root.getLeft()), height(root.getRight())));

    // Calculate the balance factor
    int balance = getBalance(root);

    // Perform rotations if needed

    // Left heavy
    if (balance > 1) {
        if (aDrone.getID() < root.getLeft().getID()) {
            return rotateRight(root);  // Right rotation
        } else {
            root.setLeft(rotateLeft(root.getLeft()));  // Left-Right rotation
            return rotateRight(root);  // Right rotation
        }
    }

    // Right heavy
    if (balance < -1) {
        if (aDrone.getID() > root.getRight().getID()) {
            return rotateLeft(root);  // Left rotation
        } else {
            root.setRight(rotateRight(root.getRight()));  // Right-Left rotation
            return rotateLeft(root);  // Left rotation
        }
    }

    return root;
}





 // Using recursive helper method to remove a drone into the AVL tree rooted at the given node
 private Drone removeNode(Drone root, int id) {
    if (root == null) {
        return root;
    }
    if (id < root.getID()) {
        root.m_left = removeNode(root.getLeft(), id);
    } else if (id > root.getID()) {
        root.m_right = removeNode(root.getRight(), id);
    } else {
        // Drone with the same ID as the one being removed
        // Handling the node based on one child or no child
       if (root.getLeft() == null && root.getRight() == null){
            return null;

       }
       if (root.getLeft() == null && root.getRight() != null ) {
        return root.getRight();
       }
       if (root.getRight() == null && root.getLeft() != null) {
        return root.getLeft();
       } else {
        Drone temp = minValueNode(root);
        root.m_id = temp.m_id;
        root.m_left = temp.m_left;
        root.m_right = temp.m_right;
        root.m_state = temp.m_state;
        root.m_type = temp.m_type;
        root.m_right = removeNode(root.m_right, temp.m_id);
       }
    }
   
    // Updating the height and balance factor
    root.setHeight(1 + Math.max(height(root.getLeft()), height(root.getRight())));
        
    // Check balance factor and perform rotations if needed
    int balance = getBalance(root);

    // Applying a left rotation
    if (balance > 1) {
        if (id < root.getLeft().getID()){
            return rotateRight(root);
        } else {
            root.setLeft(rotateLeft(root.getLeft()));
            return rotateRight(root);
        }
    }
    // Applying a right rotation
    if (balance < -1) {
        if (id > root.getRight().getID()) {
            return rotateLeft(root);
        } else {
            root.setRight(rotateRight(root.getRight()));
            return rotateLeft(root);
        }
    } 
    return root;
}

// Using a recursive helper function to perform a left rotation
private Drone rotateLeft(Drone x) {
    if (x == null || x.m_right == null) {
        // No rotation needed if x or x's right child is null
        return x;
    }

    Drone y = x.m_right;
    Drone T2 = y.m_left;

    y.m_left = x;
    x.m_right = T2;

    x.m_height = 1 + Math.max(height(x.m_left), height(x.m_right));
    y.m_height = 1 + Math.max(height(y.m_left), height(y.m_right));

    return y;
}


// Using a recursive helper function to perform a right rotation
private Drone rotateRight(Drone y) {
    Drone x = y.m_left;
    Drone T2 = x.m_right;

    x.m_right = y;
    y.m_left = T2;

    y.m_height = 1 + Math.max(height(y.m_left), height(y.m_right));
    x.m_height = 1 + Math.max(height(x.m_left), height(x.m_right));

    return x;
}


// Using recursive helper method to list drones in ascending order 
private String listDronesInOrder(Drone root) {
    if (root == null) {
        return "";
    }

    String result = "";

    // Traverse the left subtree
    result += listDronesInOrder(root.getLeft());

    // Append the drone information
    if (!result.isEmpty()) {
        result += "\n"; // Add a newline character if it's not the first drone
    }
    result += root.getID() + ":" + root.getState() + ":" + root.getType();

    // Traverse the right subtree
    String rightSubtree = listDronesInOrder(root.getRight());
    
    if (!rightSubtree.isEmpty()) {
        result += "\n" + rightSubtree; // Add a newline character here if there is a right subtree
    }

    return result;
}


// Using a recursive helper function to find the node with the minimum value in the tree
private Drone minValueNode(Drone node) {
    Drone current = node;
    while (current.m_left != null) {
        current = current.m_left;
    }
    return current;
}


// Using recursive helper method to update the state of a drone
private boolean setDroneState(Drone root, int id, STATE state) {
    if (root == null) {
        return false;
    }
    if (id < root.getID()) {
        return setDroneState(root.getLeft(), id, state);
    } else if (id > root.getID()) {
        return setDroneState(root.getRight(), id, state);
    } else {
        // Drone with the same ID as the one being listed
        root.setState(state);
        return true;
    }
  }
    
 // Using recursive helper method to remove drones with LIGHTOFF state 
 private Drone removeLightOffDrones(Drone root) {
    if (root == null) {
        return root;
    }

    // Recursively remove LIGHTOFF drones from the left and right subtrees
    root.setLeft(removeLightOffDrones(root.getLeft()));
    root.setRight(removeLightOffDrones(root.getRight()));

    // Check if the current node (root) is a LIGHTOFF drone
    if (root != null && root.getState() == STATE.LIGHTOFF) {
        // Remove the current node and update the root accordingly
        root = removeNode(root, root.getID());
    }

    if (root != null) {
        // Update the height of the current node
        root.setHeight(1 + Math.max(height(root.getLeft()), height(root.getRight())));

        // Rebalance the tree
        int balance = getBalance(root);

        // Perform rotations if needed
        if (balance > 1) {
            if (getBalance(root.getLeft()) >= 0) {
                root = rotateRight(root);
            } else {
                root.setLeft(rotateLeft(root.getLeft()));
                root = rotateRight(root);
            }
        } else if (balance < -1) {
            if (getBalance(root.getRight()) <= 0) {
                root = rotateLeft(root);
            } else {
                root.setRight(rotateRight(root.getRight()));
                root = rotateLeft(root);
            }
        }
    }

    return root;
}



// Using recursive helper method to count drones by LIGHTCOLOR 
private int countDronesByColor(Drone root, LIGHTCOLOR aColor) {
    if (root == null) {
        return 0;
    }

    int count = 0;
    if (root.getType() == aColor) {
        count = 1;
    }
    // Recursively count drones of the specified color in the left and right subtrees.
    int leftCount = countDronesByColor(root.getLeft(), aColor);
    int rightCount = countDronesByColor(root.getRight(), aColor);

    return count + leftCount + rightCount;
}
// Using recursive helper method to drones by ID
private boolean findDroneByID(Drone root, int id) {
        if (root == null) {
            return false;
        }
    if (id == root.getID()) {
        return true;
    }
    if (id < root.getID()) {
        return findDroneByID(root.getLeft(), id);
    } else {
        return findDroneByID(root.getRight(), id);
    }
}

// Using recursive function to get the balance factor of a node
private int getBalance(Drone node) {
    if (node == null) {
        return 0;
    }
    return height(node.getLeft()) - height(node.getRight());
}
// Using recursive function to 
    class Drone {
        // public:
        // friend class Show;
        // friend class Grader;
        // friend class Tester;
        public LIGHTCOLOR DEFAULT_LIGHT = LIGHTCOLOR.RED;
        public STATE DEFAULT_STATE = STATE.LIGHTON;

        /**
         * Conversion constructor for Drone
         * sets id to parmater id
         * sets type to the default light
         * sets state to the default state
         * @param id id for the Drone
         */
        public Drone(int id) {
            m_id = id;
            m_type = DEFAULT_LIGHT;
            m_state = DEFAULT_STATE;
            m_left = null;
            m_right = null;
            m_height = DEFAULT_HEIGHT;
        }

        /**
         * Parameterized contructor for default values
         * @param id Drone id
         * @param type Drone light color
         * @param state Drone state (lightOn, lightOff)
         */
        public Drone(int id, LIGHTCOLOR type, STATE state) {
            m_id = id;
            m_type = type;
            m_state = state;
            m_left = null;
            m_right = null;
            m_height = DEFAULT_HEIGHT;
        }
        /**
         * Default contructor
         * sets to default id
         * sets the type to the default type
         * sets the state to the default state
         */
        public Drone() {
            m_id = DEFAULT_ID;
            m_type = DEFAULT_LIGHT;
            m_state = DEFAULT_STATE;
            m_left = null;
            m_right = null;
            m_height = DEFAULT_HEIGHT;
        }

        /**
         * Returns the Drone id
         * @return the Drone id
         */
        int getID() {
            return m_id;
        }

        /**
         * Returns the Drone state
         * @return the Drone state
         */
        STATE getState() {
            return m_state;
        }

        /**
         * Converts a state to it's string representation
         * @return string representation of state
         */
        String getStateStr() {
            String text = "";
            switch (m_state) {
                case LIGHTOFF:
                    text = "LIGHTOFF";
                    break;
                case LIGHTON:
                    text = "LIGHTON";
                    break;
                default:
                    text = "UNKNOWN";
                    break;
            }
            return text;
        }

        /**
         * Returns the type of the Drone
         * @return Drone type
         */
        LIGHTCOLOR getType() {
            return m_type;
        }

        /**
         * Converts the type to a string representation
         * @param string representation of type
         */
        String getTypeStr() {
            String text = "";
            switch (m_type) {
                case RED:
                    text = "RED";
                    break;
                case GREEN:
                    text = "GREEN";
                    break;
                case BLUE:
                    text = "BLUE";
                    break;
                default:
                    text = "UNKNOWN";
                    break;
            }
            return text;
        }

        /**
         * Returns the height of the Drone in the tree
         * @return Drone height in the tree
         */
        int getHeight() {
            return m_height;
        }

        /**
         * Returns a reference to the left child
         * @return a refernce to the left child
         */
        Drone getLeft() {
            return m_left;
        }

        /**
         * Returns a reference to the right child
         * @return a reference to the right child
         */
        Drone getRight() {
            return m_right;
        }

        /**
         * Set the id for the Drone
         * @param id id for the Drone
         */
        void setID(int id) {
            m_id = id;
        }

        /**
         * Sets the state for the Drone
         * @param state state for the Drone
         */
        void setState(STATE state) {
            m_state = state;
        }

        /**
         * Sets the type (lightcolor) for the Drone
         * @param type the (lightcolor) for the Drone
         */
        void setType(LIGHTCOLOR type) {
            m_type = type;
        }

        /**
         * Sets the height for the Drone
         * @param height the height for the Drone
         */
        void setHeight(int height) {
            m_height = height;
        }

        /**
         * Sets the left child of the Drone
         * @param left reference to be set as left of the Drone
         */
        void setLeft(Drone left) {
            m_left = left;
        }

        /**
         * Sets the right child of the Drone
         * @param right reference to be set as the right of the Drone
         */
        void setRight(Drone right) {
            m_right = right;
        }

        private int m_id;
        private LIGHTCOLOR m_type;
        private STATE m_state;
        private Drone m_left;// the pointer to the left child in the BST
        private Drone m_right;// the pointer to the right child in the BST
        private int m_height;// the height of node in the BST
    }// end of class Drone
}// end of class Show