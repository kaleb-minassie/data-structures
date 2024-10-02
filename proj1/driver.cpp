// UMBC - CMSC 341 - Fall 2024 - Proj1
#include "snakes.h"
using namespace std;
class Tester{
public:
};
int main(){
    cout << "Creating a 5x5 board with a default number of snakes and ladders:" << endl << endl;
    Snakes game;
    game.dumpBoard();
   cout<<"Rolling dice"<<endl;
   for(int i = 0; i <10; ++i)
   {
    int roll = game.rollDice();
    cout<< "Roll " << (i+1)  << ":"<< roll <<endl;
   }
    cout << "Player 1 gets a 3, and player 2 gets a 6:" << endl << endl;
    game.play(3);// player 1
    game.play(6);// player 2
    game.dumpBoard();
   
    return 0;
}