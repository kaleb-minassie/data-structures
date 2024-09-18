#include "solitaire.h"
#include <iostream>

class Tester {
public:
    // Test cases
    bool testConstructorErrorCase();
    bool testConstructorNormalCase();
    bool testNewBoardErrorCase();
    bool testNewBoardNormalCase();
    bool testChangeBoardSameShape();
    bool testChangeBoardDifferentShape();
    bool testPlayValidMove();
    bool testPlayInvalidMove();
    bool testCopyConstructorErrorCase();
    bool testCopyConstructorNormalCase();
};

bool Tester::testConstructorErrorCase() {
    Solitaire game(FRENCH);  // FRENCH is unsupported
    if (game.m_shape == NONE && game.m_board == nullptr) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

bool Tester::testConstructorNormalCase() {
    Solitaire game(ENGLISH);
    if (game.m_shape == ENGLISH && game.m_board != nullptr && game.m_numMarbles == NUMENGLISHMARBLES) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

bool Tester::testNewBoardErrorCase() {
    Solitaire game;  // Default constructor, initially empty
    if (!game.newBoard()) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

bool Tester::testNewBoardNormalCase() {
    Solitaire game(ENGLISH);  // Create a game with English board setup
    game.clear();  // Clear the game
    bool result = game.newBoard();  // Attempt to reinitialize the board
    if (result && game.m_shape == ENGLISH && game.m_numMarbles == NUMENGLISHMARBLES &&
        game.m_board[ENGLISHSIZE / 2][ENGLISHSIZE / 2] == HOLE) { // Also, check for proper setup
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}


bool Tester::testChangeBoardSameShape() {
    Solitaire game(ENGLISH);
    int** oldBoard = game.m_board;  // Store the old board address
    game.changeBoard(ENGLISH);  // Attempt to change to the same board shape
    if (game.m_shape == ENGLISH && game.m_board == oldBoard) {  // Check if the board address is the same
        cout << "You PASSED the unit test\n\n";
        return true;  // Expected no change in board memory address
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}


bool Tester::testChangeBoardDifferentShape() {
    Solitaire game(ENGLISH);
    game.changeBoard(DIAMOND);
    if (game.m_shape == DIAMOND) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

bool Tester::testPlayValidMove() {
    Solitaire game(ENGLISH);
    game.m_board[3][3] = HOLE;
    game.m_board[3][4] = MARBLE;
    game.m_board[3][5] = MARBLE;
    if (game.play(make_pair(3, 5), make_pair(3, 3))) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}
bool Tester::testPlayInvalidMove() {
    Solitaire game(ENGLISH);
    if (!game.play(make_pair(3, 3), make_pair(3, 6))) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

bool Tester::testCopyConstructorErrorCase() {
    Solitaire empty;
    Solitaire copy(empty);
    if (copy.m_board == nullptr && copy.m_shape == NONE) {
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

bool Tester::testCopyConstructorNormalCase() {
    Solitaire game(ENGLISH);
    Solitaire copy(game);
    if (copy.m_numColumns == game.m_numColumns &&
        copy.m_numRows == game.m_numRows &&
        copy.m_shape == game.m_shape &&
        copy.m_numMarbles == game.m_numMarbles &&
        copy.m_board != game.m_board) {
        for (int i = 0; i < game.m_numRows; i++) {
            for (int j = 0; j < game.m_numColumns; j++) {
                if (copy.m_board[i][j] != game.m_board[i][j]) {
                    cout << "You DID NOT PASS the unit test\n\n";
                    return false;
                }
            }
        }
        cout << "You PASSED the unit test\n\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test\n\n";
        return false;
    }
}

int main() {
    Tester tester;
    cout << "UNIT TEST - Constructor Error Case" << endl;
    tester.testConstructorErrorCase();
    cout << "UNIT TEST - Constructor Normal Case" << endl;
    tester.testConstructorNormalCase();
    cout << "UNIT TEST - New Board Error Case" << endl;
    tester.testNewBoardErrorCase();
    cout << "UNIT TEST - New Board Normal Case" << endl;
    tester.testNewBoardNormalCase();
    cout << "UNIT TEST - Change Board Same Shape" << endl;
    tester.testChangeBoardSameShape();
    cout << "UNIT TEST - Change Board Different Shape" << endl;
    tester.testChangeBoardDifferentShape();
    cout << "UNIT TEST - Play Valid Move" << endl;
    tester.testPlayValidMove();
    cout << "UNIT TEST - Play Invalid Move" << endl;
    tester.testPlayInvalidMove();
    cout << "UNIT TEST - Copy Constructor Error Case" << endl;
    tester.testCopyConstructorErrorCase();
    cout << "UNIT TEST - Copy Constructor Normal Case" << endl;
    tester.testCopyConstructorNormalCase();
    return 0;
}
