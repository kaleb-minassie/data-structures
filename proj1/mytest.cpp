// UMBC - CMSC 341 - Fall 2024 - Proj1
#include "snakes.h"
#include <iostream>
using namespace std;
class Tester {
public:
    // Test cases
    bool testConstructorDefault();
    bool testConstructorBoardSize();
    bool testMakeDefaultBoardNormal();
    bool testMakeDefaultBoardError();
    bool testMakeRandomBoard();
    bool testLadderAndSnake();
    bool testPlayValidMove();
    bool testPlayInvalidMove();
    bool testRestartGame();
    bool testRollDice();
    bool testAssignmentOperator();
};


//test default constructor
bool Tester::testConstructorDefault() {
    Snakes game;
    if (game.whoseTurn() == NOGAME) { //check if game state is NOGAME
        cout << "You PASSED the unit test for default constructor.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for default constructor.\n";
        return false;
    }
}

//test constructor with board size
bool Tester::testConstructorBoardSize() {
    Snakes game(16); //4x4 board
    if (game.whoseTurn() == PLAYER1TURN) { //check if Player 1 starts
        cout << "You PASSED the unit test for constructor with board size.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for constructor with board size.\n";
        return false;
    }
}

//test valid default board creation
bool Tester::testMakeDefaultBoardNormal() {
    Snakes game;
    if (game.makeDefaultBoard(16)) { //4x4 board
        cout << "You PASSED the unit test for makeDefaultBoard.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for makeDefaultBoard.\n";
        return false;
    }
}

//test invalid default board size
bool Tester::testMakeDefaultBoardError() {
    Snakes game;
    if (!game.makeDefaultBoard(20)) { //invalid size, not a perfect square
        cout << "You PASSED the unit test for invalid makeDefaultBoard.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for invalid makeDefaultBoard.\n";
        return false;
    }
}

//test random board creation
bool Tester::testMakeRandomBoard() {
    Snakes game;
    game.makeRandomBoard(16, 4); //4x4 board, 4 snakes/ladders
    cout << "You PASSED the unit test for makeRandomBoard.\n";
    return true;
}

//test player movement for snake and ladder
bool Tester::testLadderAndSnake() {
    {
        Snakes game(9); // 3x3 board

        //clear the board
        game.clear();

        //create a default board
        game.makeDefaultBoard(9);

        //move Player 1 to a ladder (assuming a ladder from cell 2 to cell 4)
        game.play(1); // player 1 moves from cell 1 to 2 (ladder)
        if (game.whoseTurn() == PLAYER2TURN && game.play(0)) { // player 1 should automatically move up the ladder to 4
            cout << "You PASSED the unit test for ladder movement.\n";
        } else {
            cout << "You DID NOT PASS the unit test for ladder movement.\n";
            return false;
        }

        //move Player 2 to a snake (assuming a snake from cell 6 to cell 3)
        game.play(3); // player 2 moves from 1 to 4
        game.play(1); // player 2 moves from 4 to 5 (then hits snake at cell 6)
        if (game.whoseTurn() == PLAYER1TURN) { // player 2 should slide down the snake
            cout << "You PASSED the unit test for snake movement.\n";
        } else {
            cout << "You DID NOT PASS the unit test for snake movement.\n";
            return false;
        }

        //again, clear the board to avoid memory leaks
        game.clear();
    } //'Snakes' object destroyed; calling the destructor

    return true;
}




//test valid play move
bool Tester::testPlayValidMove() {
    Snakes game(9); //3x3 board
    bool result = game.play(2); //Player 1 moves 2 spaces
    if (result && game.whoseTurn() == PLAYER2TURN) { //check if Player 2's turn
        cout << "You PASSED the unit test for valid play.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for valid play.\n";
        return false;
    }
}

//test play without active game
bool Tester::testPlayInvalidMove() {
    Snakes game; //no board created
    bool result = game.play(3); //attempt invalid move
    if (!result) { //should return false
        cout << "You PASSED the unit test for invalid play.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for invalid play.\n";
        return false;
    }
}

//test restarting the game
bool Tester::testRestartGame() {
    Snakes game(16); //4x4 board
    game.play(3); //play first move
    game.reStart(); //restart game
    if (game.whoseTurn() == PLAYER1TURN) { // Check if Player 1 starts again
        cout << "You PASSED the unit test for reStart.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for reStart.\n";
        return false;
    }
}

//test dice roll
bool Tester::testRollDice() {
    Snakes game;
    int roll = game.rollDice(); //roll the dice
    if (roll >= 1 && roll <= 6) { //check if dice is between 1 and 6
        cout << "You PASSED the unit test for rollDice.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for rollDice.\n";
        return false;
    }
}

//test assignment operator
bool Tester::testAssignmentOperator() {
    Snakes game1(16); //4x4 board
    Snakes game2;
    game2 = game1; //assigns game1 to game2
    if (game2.whoseTurn() == game1.whoseTurn()) { // check if turns are copied
        cout << "You PASSED the unit test for assignment operator.\n";
        return true;
    } else {
        cout << "You DID NOT PASS the unit test for assignment operator.\n";
        return false;
    }
}

int main() {
    Tester tester;
    cout << "UNIT TEST - Default Constructor\n";
    tester.testConstructorDefault();
    cout << "UNIT TEST - Constructor with Board Size\n";
    tester.testConstructorBoardSize();
    cout << "UNIT TEST - Make Default Board Normal Case\n";
    tester.testMakeDefaultBoardNormal();
    cout << "UNIT TEST - Make Default Board Error Case\n";
    tester.testMakeDefaultBoardError();
    cout << "UNIT TEST - Make Random Board\n";
    tester.testMakeRandomBoard();
    cout << "UNIT TEST - Play Valid Move\n";
    tester.testPlayValidMove();
    cout << "UNIT TEST - Ladder and Snake Movement\n";
    tester.testLadderAndSnake();
    cout << "UNIT TEST - Play Invalid Move\n";
    tester.testPlayInvalidMove();
    cout << "UNIT TEST - Restart Game\n";
    tester.testRestartGame();
    cout << "UNIT TEST - Roll Dice\n";
    tester.testRollDice();
    cout << "UNIT TEST - Assignment Operator\n";
    tester.testAssignmentOperator();
}
