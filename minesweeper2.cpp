#include <iostream>
#include <string>
#include <stdlib.h> // srand, rand
#include <list>
using namespace std;

class board {
    
    private:
        char** boardArray;
        int boardX,boardY;
        std::list<std::pair<int,int> > mineList;

        //isSafe
        //checks if there already exists a mine at the given x,y
        //in: coordinate x,y 
        bool isSafe(int x, int y) {
            bool result = true;
            std::list<std::pair<int,int> >::iterator iter;
            for (iter = mineList.begin(); iter != mineList.end(); ++iter) {
                cout << "comparing: " << x << ", " << y << " with " << iter->first << ", " << iter->second << endl;
                if (x == iter->first && y == iter->second) {
                    result = false;
                      break;
                    }
            }
            return result;
        }

        //createMine
        //randomly picks an X and Y pos for a mine.
        //checks if the pos is already used from blacklist and picks another if it is.
        //in: coordinate x,y
        void createMine(int x, int y) {
            int randX = rand() % y;
            int randY = rand() % x;
            cout << "Trying " << randX << ", " << randY << endl; 
            while (!isSafe(randX, randY)) {
                randX = rand() % y;
                randY = rand() % x;
            }
            mineList.push_back(std::make_pair(randX,randY));
            cout << "Created a mine: " << randX << ", " << randY << endl;
        }

        //creates all mines by calling createMine.  
        void createAllMines(int x, int y, int num_of_mines) {
            srand (time(NULL));
            for (int i = 0; i < num_of_mines; ++i) {
               createMine(x,y);
            }
        }

        void explode() {
            cout << "BIG OL' BOOOOM YOU DIE GG" << endl;
        }

        void clearSpot(int x, int y) {
            boardArray[x][y] = 'O';
            
        }

    public: 
        board(int x, int y, int num_of_mines) {
            boardArray = new char*[x];
            boardX = x;
            boardY = y;
            for (int i = 0; i < x; ++i) {
                boardArray[i] = new char[y];
                for (int j = 0; j < y; ++j) {
                    boardArray[i][j] = '@';
                }
            }
            createAllMines(x,y,num_of_mines);
        }
        
        void touchSpot(int x, int y) {
            cout << "touching " << x << ", " << y << endl;
            if (!isSafe(x,y)) {
                //fukkin BOOOM, yo
                explode();
            } else {
                clearSpot(x,y);
            }
        }

        void displayBoard() {
                int rowNum = 1;
                for (int i = 0; i < boardY; ++i) {
                    cout << i;
                    for (int j = 0; j < boardX; ++j) {
                        cout << " " << boardArray[i][j];
                    }
                    cout << endl;
                }
                cout << " "; 
                for (int i = 0; i < boardY; ++i){
                    cout << " " << i ;
                }
                cout << endl;
        }
            
};


int main() {
    cout << "main" << endl;

    int givenX = 10;
    int givenY = 10;
    int givenMineCount = 6;
    board gameBoard = *new board(givenX,givenY,givenMineCount);
    gameBoard.displayBoard();
    gameBoard.touchSpot(5,5);
    gameBoard.displayBoard();
}
