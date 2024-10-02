// UMBC - CMSC 341 - Fall 2024 - Proj1
#include "snakes.h"
//default constructor
Snakes::Snakes():m_dice(1,6){ //initializes m_dice with min=1, max=6
    m_start=0; //start of board to null
    m_playerTurn=NOGAME; //game state to NOGAME
    m_player1=0; //player 1 to no position
    m_player2=0; //player 2 to no position
}
//destructor
Snakes::~Snakes(){
    clear();
}
//clears the memory for all board cells
void Snakes::clear(){
    Cell *temp;
    while (m_start != nullptr) {
        temp = m_start;         //holds the current node
        m_start = m_start->getNext();  //moves to the next node
        delete temp;            //deletes the current node
    }
    m_start = nullptr;          //resets the start pointer
    m_player1 = nullptr;        //resets player positions
    m_player2 = nullptr;
    m_playerTurn = NOGAME;      //resets game state

}
//overloaded assignment operator (deep copy)
const Snakes & Snakes::operator=(const Snakes & rhs) {
    if (this != &rhs) {  //avoid self-assignment
        clear();  //clear current object to avoid memory leaks

        //copy the board from rhs
        Cell* currentRhs = rhs.m_start;
        Cell* prevNew = nullptr;
        while (currentRhs != nullptr) {
            //create a new cell for the current node
            Cell* newCell = new Cell(currentRhs->m_cellID);

            //link the new cell to the previous cell
            if (m_start == nullptr) {
                m_start = newCell;  //set the first cell
            } else {
                prevNew->setNext(newCell);  //link the cells
            }
            prevNew = newCell;

            //copy the ladder and snake pointers (m_north and m_south) from rhs
            newCell->setNorth(currentRhs->getNorth());  //copy the ladder pointer (north)
            newCell->setSouth(currentRhs->getSouth());  //copy the snake pointer (south)

            currentRhs = currentRhs->getNext();  //move to the next cell
        }

        //copy player positions
        m_player1 = m_start;
        m_player2 = m_start;
        m_playerTurn = rhs.m_playerTurn;  //copy the player turn from rhs
    }
    return *this;  //return the current object
}


//simulates rolling a dice
int Snakes::rollDice(){
    return m_dice.getRandNum(); //generate a number between 1 and 6
}
//constructor to create a default board with the specified size
Snakes::Snakes(int boardSize){
    int sqrtSize = (int) sqrt(boardSize);  //checks if its a perfect square
    if (sqrtSize * sqrtSize != boardSize) {
        //adjusts to the largest size that has an integer square root
        boardSize = sqrtSize * sqrtSize;
    }
    if (!makeDefaultBoard(boardSize)) {  //to create the board
        clear();  
    }
}

//creates a default board with snakes and ladders according to the game rules
bool Snakes::makeDefaultBoard(int boardSize){
    int sqrtSize = (int)sqrt(boardSize);  // calculates square root of boardSize
    if (sqrtSize * sqrtSize != boardSize || boardSize < 9) {  // checks if it's a valid square board
        return false;
    }

    // creates the board with cells
    m_start = new Cell(1);
    Cell* current = m_start;
    for (int i = 2; i <= boardSize; i++) {
        Cell* newCell = new Cell(i);
        current->setNext(newCell);
        current = newCell;
    }

    // calculates the number of snakes and ladders
    int numSnakesLadders = boardSize / 4;
    if (numSnakesLadders % 2 != 0) {
        numSnakesLadders++;  // adjusts for even number of snakes/ladders
    }

    // inserts ladders (going up)
    Cell* temp = m_start->getNext();  // skips the first cell
    for (int i = 0; i < numSnakesLadders / 2; i++) {
        Cell* ladderStart = temp;
        Cell* ladderEnd = temp->getNext()->getNext();  // ensures the ladder goes to a higher cell

        ladderStart->setNorth(ladderEnd);  // create a ladder
        temp = temp->getNext()->getNext();  // move forward for the next ladder
    }

    // inserts snakes (going down)
    temp = m_start;  // reset temp to start
    for (int i = 0; i < numSnakesLadders / 2; i++) {
        // find a higher cell for the snake to start
        Cell* snakeStart = temp->getNext()->getNext();  // move forward to a higher cell
        Cell* snakeEnd = temp;  // snake should go down to a lower-numbered cell

        snakeStart->setSouth(snakeEnd);  // create a snake
        temp = temp->getNext()->getNext();  // move forward for the next snake
    }

    // initialize players' starting positions
    m_player1 = m_start;
    m_player2 = m_start;
    m_playerTurn = PLAYER1TURN;  // player 1 starts the game

    return true;
}


//creates a random board with snakes and ladders according to the game rules using a linked list
void Snakes::makeRandomBoard(int boardSize, int numSnakesLadders) {
    int sqrtSize = (int) sqrt(boardSize);  // check if it's a perfect square
    if (sqrtSize * sqrtSize != boardSize) {
        //adjust board size to the largest valid square board less than boardSize
        boardSize = sqrtSize * sqrtSize;
    }

    clear();  //clear the current board before creating a new one

    //create the new board with cells
    m_start = new Cell(1);  //create the first cell (starting position)
    Cell* current = m_start;
    for (int i = 2; i <= boardSize; i++) {  //loop to create the rest of the cells
        Cell* newCell = new Cell(i);  //create a new cell
        current->setNext(newCell);  //link the new cell to the previous one
        current = newCell;  //move to the next cell
    }

    //adjust numSnakesLadders to fit within valid limits
    int maxSnakesLadders = (boardSize / 2) - 2;  //cannot exceed half the board size minus 2
    if (numSnakesLadders < 2) {
        numSnakesLadders = 2;  //set minimum number of snakes/ladders
    } else if (numSnakesLadders > maxSnakesLadders) {
        numSnakesLadders = maxSnakesLadders;  //cap to max allowed number of snakes/ladders
    }

    Random randGen(2, boardSize - 1);  //create a random number generator for valid cells (excluding 1st and last)

    //we will loop to set ladders and snakes based on the number of ladders and snakes required
    int half = numSnakesLadders / 2;
    for (int i = 0; i < half; i++) {
        //random placement for ladders
        int startLadder = randGen.getRandNum();  //get random starting position for ladder
        int endLadder = randGen.getRandNum();  //get random ending position for ladder

        while (startLadder >= endLadder) {  //ensure ladders move up
            startLadder = randGen.getRandNum();
            endLadder = randGen.getRandNum();
        }

        //find the start and end cells for the ladder using the linked list
        Cell* startLadderCell = m_start;
        for (int j = 1; j < startLadder; j++) {
            startLadderCell = startLadderCell->getNext();
        }
        Cell* endLadderCell = m_start;
        for (int j = 1; j < endLadder; j++) {
            endLadderCell = endLadderCell->getNext();
        }

        startLadderCell->setNorth(endLadderCell);  // set the ladder

        //random placement for snakes
        int startSnake = randGen.getRandNum();  // get random starting position for snake
        int endSnake = randGen.getRandNum();  // get random ending position for snake

        while (startSnake <= endSnake) {  // ensure snakes move down
            startSnake = randGen.getRandNum();
            endSnake = randGen.getRandNum();
        }

        //find the start and end cells for the snake using the linked list
        Cell* startSnakeCell = m_start;
        for (int j = 1; j < startSnake; j++) {
            startSnakeCell = startSnakeCell->getNext();
        }
        Cell* endSnakeCell = m_start;
        for (int j = 1; j < endSnake; j++) {
            endSnakeCell = endSnakeCell->getNext();
        }

        startSnakeCell->setSouth(endSnakeCell);  // set the snake
    }

    //initialize player positions
    m_player1 = m_start;  //set player 1 to the start position
    m_player2 = m_start;  //set player 2 to the start position
    m_playerTurn = PLAYER1TURN;  //player 1 starts the game
}



//moves a player based on the dice roll
bool Snakes::play(int dice){
    Cell* currentPlayer;  //pointer to track the current player's position
    
    //determines which player's turn it is
    if (m_playerTurn == PLAYER1TURN) {
        currentPlayer = m_player1;  //it's player 1's turn
    } else if (m_playerTurn == PLAYER2TURN) {
        currentPlayer = m_player2;  //it's player 2's turn
    } else {
        return false;  //no active game, return false
    }

    //move the player forward based on the dice roll
    for (int i = 0; i < dice && currentPlayer->getNext() != nullptr; i++) {
        currentPlayer = currentPlayer->getNext();  //move to the next cell
    }

    //check if the player landed on a ladder or a snake
    if (currentPlayer->getNorth() != nullptr) {
        currentPlayer = currentPlayer->getNorth();  //player climbs the ladder
    } else if (currentPlayer->getSouth() != nullptr) {
        currentPlayer = currentPlayer->getSouth();  //player slides down the snake
    }

    //update the player's position after the move
    if (m_playerTurn == PLAYER1TURN) {
        m_player1 = currentPlayer;  //update player 1's position
        m_playerTurn = PLAYER2TURN;  //switch turn to player 2
    } else {
        m_player2 = currentPlayer;  //update player 2's position
        m_playerTurn = PLAYER1TURN;  //switch turn to player 1
    }

    //check if the game is over (if a player reaches the last cell)
    if (currentPlayer->getNext() == nullptr) {
        return false;  //game over, a player reached the last cell
    }

    return true;  //game continues

}
//restarts the game w/o changing the board
void Snakes::reStart(){
    m_player1 = m_start;  // resets player 1 to the start
    m_player2 = m_start;  // resets player 2 to the start
    m_playerTurn = PLAYER1TURN;  // Player 1 starts the game
}
void Snakes::dumpBoard(){
    if (m_start != nullptr){
        int ladderCode, snakeCode;
        Cell *temp = m_start;
        cout << "START => ";
        while(temp != nullptr){
            cout << temp->m_cellID;

            if (temp == m_player1) cout << " (Player 1)";
            if (temp == m_player2) cout << " (Player 2)";
            if (temp->m_north != nullptr) ladderCode = temp->m_north->m_cellID;
            else ladderCode = -1;
            if (temp->m_south != nullptr) snakeCode = temp->m_south->m_cellID;
            else snakeCode = -1;
            if (ladderCode != -1 || snakeCode != -1){
                cout << " (";
                if (ladderCode != -1) cout << "\033[1;32mLadder to: " << ladderCode << "\033[0m";// green text
                if (snakeCode != -1) cout << "\033[1;31mSnake to: " << snakeCode << "\033[0m";//red text
                cout << ")";
            }
            cout << " => ";
            temp = temp->m_next;
        }
        cout << "END" << endl;
    }
}
