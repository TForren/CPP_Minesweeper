#include <iostream>
#include <string>
#include <stdlib.h> // srand, rand
#include <list>
#include <vector>
using namespace std;

class board {
    
    private:
        std::vector<std::vector<char> > boardArray;
        int boardX,boardY;
        int gameState; //0: in progress 1: loss 2: win
        std::list<std::pair<int,int> > mineList;

        bool isValid(int x, int y) {
            bool result = true;
            
            if ( x >= boardX || x < 0 || y >= boardY || y < 0) {
                result = false;
            } else if (!(boardArray[x][y] == '@')) {
                result = false;
            }
            return result;
        }

        //isSafe
        //checks if there already exists a mine at the given x,y
        //in: coordinate x,y 
        bool isSafe(int x, int y) {
            bool result = true;
            std::list<std::pair<int,int> >::iterator iter;
            for (iter = mineList.begin(); iter != mineList.end(); ++iter) {
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
            while (!isSafe(randX, randY)) {
                randX = rand() % y;
                randY = rand() % x;
            }
            mineList.push_back(std::make_pair(randX,randY));
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
            gameState = 1;
        }

        //clears the current spot and recurses over other completely clear spots 
        void clearSpot(int x, int y) {
            //boardArray[x][y] = '-';
            int mineCount = 0;
            std::list<std::pair<int,int> > todo;

            //check all surrounding squares of the current
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (!isValid(x+i,y+j)) {
                        continue;
                    } else {
                        if (!isSafe(x+i,y+j)) {
                            mineCount += 1;
                        } else {
                            todo.push_back(std::make_pair(x+i,y+j));
                        }
                    }
                }
            }
            if (mineCount == 0) {
                boardArray[x][y] = '-';
                //recurse over the adjacent non-mine spaces
                std::list<std::pair<int,int> >::iterator iter;
                for (iter = todo.begin(); iter != todo.end(); ++iter) {
                        clearSpot(iter->first,iter->second);
                }
            } else {
                boardArray[x][y] = '0' + mineCount;
            }
        }

        void checkWin() {
            int coverCount = 0;
            for (int i = 0; i < boardY; ++i) {
                for (int j = 0; j < boardX; ++j) {
                    if (boardArray[i][j] == '@') {
                        ++coverCount;
                    }
                }
            }
            if (mineList.size() >= coverCount) {
                gameState = 2;
                cout << " ";
                for (int i = 0; i < (boardX / 2)+3; ++i) {cout << "==";}
                cout << "YOU WIN!";
                for (int i = 0; i < (boardX / 2)+3; ++i) {cout << "==";}
                cout << endl;
            }
        }

    public: 
        board(int x, int y, int num_of_mines) {
            boardArray.resize(x, std::vector<char>(y, '@'));
            boardX = x;
            boardY = y;
            createAllMines(x,y,num_of_mines);
        }
        
        void touchSpot(int x, int y) {
            cout << "sweeping: " << x << ", " << y << endl;
            if (!isSafe(x,y)) {
                explode();
            } else {
                clearSpot(x,y);
            }
            checkWin();
        }

        void displayBoard() {
            int rowNum = 1;
            for (int i = 0; i < boardY; ++i) {
                cout << i;
                if (i < 10) { cout << "  "; } else { cout << " "; };   
                for (int j = 0; j < boardX; ++j) {
                    cout << boardArray[j][i] << "  ";
                }
                cout << endl;
            }
            cout << "   ";
            for (int i = 0; i < boardX; ++i) {
                cout << i;
                if (i < 10) {cout << "  ";} else { cout << " ";};
            }
            cout << endl;
        }
        
        void acceptTouchLocation() {
            int x,y;
            cout << "x y: ";
            cin >> x >> y;
            

            if (!isValid(x,y)) {
                cout << "Invalid coordinate. Please enter a different coordinate" << endl;
            } else {
                touchSpot(x,y);
            }
        }

        int getGameState() {
            return gameState;
        }
};


int main(int argc,char *argv[]) {
    int givenX = atoi(argv[1]);
    int givenY = atoi(argv[2]);
    int givenMineCount = atoi(argv[3]);
    cout <<"New Board: [" << givenX << ", " << givenY << "] | mines: " << givenMineCount << endl;
    board gameBoard = *new board(givenX,givenY,givenMineCount);
    gameBoard.displayBoard();
    
    do {
        gameBoard.acceptTouchLocation();
        if (gameBoard.getGameState() != 1) { gameBoard.displayBoard();};
    } while (gameBoard.getGameState() == 0);
}
