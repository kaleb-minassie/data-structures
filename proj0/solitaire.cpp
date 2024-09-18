// UMBC - CMSC 341 - Fall 2024 - Proj0
#include "solitaire.h"

// default constructor
Solitaire::Solitaire(){
     m_numRows = 0;
    m_numColumns = 0;
    m_board = nullptr;   // no board allocated
    m_shape = NONE;
    m_numMarbles = 0;
} 
// constructor w/ board shape
Solitaire::Solitaire(BOARDSHAPE board) {
    // Start with no board
    m_board = nullptr;
    
    // Check which board shape is requested
    if (board == ENGLISH || board == DIAMOND) {
        m_shape = board;
        if (board == ENGLISH) {
            m_numRows = m_numColumns = ENGLISHSIZE;  // Typically 7x7 for English
            m_numMarbles = NUMENGLISHMARBLES;        // Typically 32 marbles
        } else { // DIAMOND
            m_numRows = m_numColumns = DIAMONDSIZE;  // Typically 9x9 for Diamond
            m_numMarbles = NUMDIAMONDMARBLES;        // Typically 40 marbles
        }

        // Allocate memory for the board
        m_board = new int*[m_numRows];
        for (int i = 0; i < m_numRows; ++i) {
            m_board[i] = new int[m_numColumns];
            std::fill_n(m_board[i], m_numColumns, MARBLE); // Initialize all to MARBLE initially
        }

        // Set up the specific layout for each board
        if (m_shape == ENGLISH) {
            // English board: Clear the four corners (3x3 each)
           // English board: Clear the four corners (2x2 each)
for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
        int cornerRow = i * (ENGLISHSIZE - 2);  // Changed from 3 to 2
        int cornerCol = j * (ENGLISHSIZE - 2);  // Changed from 3 to 2
        for (int k = 0; k < 2; ++k) {  // Changed from 3 to 2
            for (int l = 0; l < 2; ++l) {  // Changed from 3 to 2
                m_board[cornerRow + k][cornerCol + l] = OUT;
            }
        }
    }
}

// Set the center as a hole
m_board[ENGLISHSIZE / 2][ENGLISHSIZE / 2] = HOLE;
        } else if (m_shape == DIAMOND) {
            // Diamond board: Set the diamond pattern
            int mid = DIAMONDSIZE / 2;
            for (int i = 0; i <= mid; ++i) {
                for (int j = 0; j < mid - i; ++j) {
                    m_board[i][j] = m_board[i][DIAMONDSIZE - 1 - j] = OUT;
                    m_board[DIAMONDSIZE - 1 - i][j] = m_board[DIAMONDSIZE - 1 - i][DIAMONDSIZE - 1 - j] = OUT;
                }
            }
            // Set the center hole
            m_board[mid][mid] = HOLE;
        }
    } else { // Invalid board shape
        // Exit with everything set to defaults for an empty object
        m_numRows = 0;
        m_numColumns = 0;
        m_numMarbles = 0;
        m_shape = NONE;
        return;
    }
}


// copy constructor
Solitaire::Solitaire(const Solitaire & rhs){
    // Copy basic attributes from the source object
    m_numRows = rhs.m_numRows;       // Number of rows in the board
    m_numColumns = rhs.m_numColumns; // Number of columns in the board
    m_shape = rhs.m_shape;           // Shape of the board (e.g., ENGLISH or DIAMOND)
    m_numMarbles = rhs.m_numMarbles; // Total number of marbles on the board

    // Check if the source object has a board initialized
    if (rhs.m_board == nullptr) {
        // If the source board is not initialized, set the current object's board to nullptr
        m_board = nullptr;
    } else {
        // If the source board is initialized, allocate memory for the current object's board
        m_board = new int*[m_numRows]; // Allocate an array of pointers for each row
        for (int i = 0; i < m_numRows; ++i) {
            // Allocate an array of integers for each column in each row
            m_board[i] = new int[m_numColumns];
            for (int j = 0; j < m_numColumns; ++j) {
                // Copy each cell value from the source board to the new board
                m_board[i][j] = rhs.m_board[i][j];
            }
        }
    }
}

// destructor
Solitaire::~Solitaire(){
    clear();
}
// clears the board and resets variables
void Solitaire::clear(){
    if (m_board != nullptr) { // checking if the board is not empty
        for (int i = 0; i < m_numRows; ++i) {
            delete[] m_board[i]; // deleting each row
        }
        delete[] m_board; // deleting the array of row pointers
        m_board = nullptr; // setting board pointer to null
    }
    // resetting member variables
    m_numRows = 0;
    m_numColumns = 0;
    m_numMarbles = 0;
    m_shape = NONE;
}

bool Solitaire::play(pair<int,int> origin, pair<int,int> destination){
    int orRow = origin.first;
    int orCol = origin.second;
    int destRow = destination.first;
    int destCol = destination.second;

    // checking boundaries
    if (orRow < 0 || orRow >= m_numRows || orCol < 0 || orCol >= m_numColumns ||
        destRow < 0 || destRow >= m_numRows || destCol < 0 || destCol >= m_numColumns) {
        return false; // move is out of bounds
    }

    // checking if move is valid (must be exactly two spaces over a marble into a hole)
    if (m_board[orRow][orCol] == MARBLE && m_board[destRow][destCol] == HOLE) {
        if (abs(orRow - destRow) == 2 && orCol == destCol) { // Vertical move
            int midRow = (orRow + destRow) / 2;
            if (m_board[midRow][orCol] == MARBLE) {
                m_board[orRow][orCol] = HOLE;
                m_board[midRow][orCol] = HOLE;
                m_board[destRow][destCol] = MARBLE;
                m_numMarbles--;
                return true;
            }
        } else if (abs(orCol - destCol) == 2 && orRow == destRow) { // Horizontal move
            int midCol = (orCol + destCol) / 2;
            if (m_board[orRow][midCol] == MARBLE) {
                m_board[orRow][orCol] = HOLE;
                m_board[orRow][midCol] = HOLE;
                m_board[destRow][destCol] = MARBLE;
                m_numMarbles--;
                return true;
            }
        }
    }
    return false; // wrong move
}

    void Solitaire::changeBoard(BOARDSHAPE board){
        clear(); // clear the current board
       // Check which board shape is requested
    if (board == ENGLISH || board == DIAMOND) {
        m_shape = board;
        if (board == ENGLISH) {
            m_numRows = m_numColumns = ENGLISHSIZE;  // Typically 7x7 for English
            m_numMarbles = NUMENGLISHMARBLES;        // Typically 32 marbles
        } else { // DIAMOND
            m_numRows = m_numColumns = DIAMONDSIZE;  // Typically 9x9 for Diamond
            m_numMarbles = NUMDIAMONDMARBLES;        // Typically 40 marbles
        }

        // Allocate memory for the board
        m_board = new int*[m_numRows];
        for (int i = 0; i < m_numRows; ++i) {
            m_board[i] = new int[m_numColumns];
            std::fill_n(m_board[i], m_numColumns, MARBLE); // Initialize all to MARBLE initially
        }

        // Set up the specific layout for each board
        if (m_shape == ENGLISH) {
            // English board: Clear the four corners (3x3 each)
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    int cornerRow = i * (ENGLISHSIZE - 3);
                    int cornerCol = j * (ENGLISHSIZE - 3);
                    for (int k = 0; k < 3; ++k) {
                        for (int l = 0; l < 3; ++l) {
                            m_board[cornerRow + k][cornerCol + l] = OUT;
                        }
                    }
                }
            }
            // Set the center as a hole
            m_board[ENGLISHSIZE / 2][ENGLISHSIZE / 2] = HOLE;
        } else if (m_shape == DIAMOND) {
            // Diamond board: Set the diamond pattern
            int mid = DIAMONDSIZE / 2;
            for (int i = 0; i <= mid; ++i) {
                for (int j = 0; j < mid - i; ++j) {
                    m_board[i][j] = m_board[i][DIAMONDSIZE - 1 - j] = OUT;
                    m_board[DIAMONDSIZE - 1 - i][j] = m_board[DIAMONDSIZE - 1 - i][DIAMONDSIZE - 1 - j] = OUT;
                }
            }
            // Set the center hole
            m_board[mid][mid] = HOLE;
        }
    } else { // Invalid board shape
        // Exit with everything set to defaults for an empty object
        m_numRows = 0;
        m_numColumns = 0;
        m_numMarbles = 0;
        m_shape = NONE;
        return;
    }
}


bool Solitaire::newBoard(){
    if (m_shape == NONE) {
        return false; // cannot reinitialize an empty board
    }
    clear(); // clear the current board
    // Reinitialize the board with the current shape following similar logic as in changeBoard
    m_numRows = m_numColumns = (m_shape == ENGLISH ? ENGLISHSIZE : DIAMONDSIZE);
    m_numMarbles = (m_shape == ENGLISH ? NUMENGLISHMARBLES : NUMDIAMONDMARBLES);

    m_board = new int*[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_board[i] = new int[m_numColumns];
        for (int j = 0; j < m_numColumns; j++) {
            m_board[i][j] = MARBLE; // Most positions start filled with marbles
        }
    }
    m_board[m_numRows / 2][m_numColumns / 2] = HOLE; // Typically, one central position starts as a hole
    return true;
}

void Solitaire::dumpBoard(){
    // this is to facilitate debugging 
    cout << "  ";
    for (int k=0;k<m_numColumns;k++){
        cout << k << " ";
    }
    cout << endl;
    for (int i=1;i <= m_numRows;i++){
        cout << i-1 << " ";
        for (int j=1;j<=m_numColumns;j++){
            if (m_board[i-1][j-1] == OUT)
                cout << "  ";
            else if (m_board[i-1][j-1] == HOLE)
                cout << BLACKSTAR;
            else
                cout << REDSTAR;
        }
        cout << endl;
    }
    cout << endl;
}
int Solitaire::reportNumMarbles(){
    // this is to facilitate debugging 
    cout << "You have " << m_numMarbles << " marbles to remove!" << endl;
    return m_numMarbles;
}

