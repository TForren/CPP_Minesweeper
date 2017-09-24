#include <iostream>
#include <string>
#include <stdlib.h> // srand, rand
#include <vector>
using namespace std;

class board {
    
    private:
        std::vector<char> boardArray;
        int boardX,boardY;
        int gameState; //0: in progress 1: loss 2: win
        std::vector<int> mineList;

        bool isValid(int x, int y) {
            bool result = true;
            if ( x >= boardX || x < 0 || y >= boardY || y < 0) {
                result = false;
            } else if (!(boardArray[x+y] == '@')) {
                result = false;
            }
            return result;
        }

        //isSafe
        //checks if there already exists a mine at the given x,y
        //in: coordinate x,y 
        bool isSafe(int x, int y) {
            bool result = true;
            std::vector<int>::iterator iter;
            for (std::vector<int>::size_type i = 0; i != mineList.size(); ++i) {
                if (mineList[i] == (x+(y*boardX))) {
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
        void createMine(int maxX, int maxY) {
            int randX = rand() % maxX;
            int randY = rand() % maxY;
            while (!isSafe(randX, randY)) {
                randX = rand() % maxX;
                randY = rand() % maxY;
            }
            mineList.push_back(randX+(randY*boardX));
            cout << "Made mine at " << randX << "+" << (randY*boardX)<< endl;
        }

        //creates all mines by calling createMine.  
        void createAllMines(int x, int y, int num_of_mines) {
            srand (time(NULL));
            cout << "Creating " << num_of_mines << " mines " << endl;
            mineList.reserve(num_of_mines);
            for (int i = 0; i < num_of_mines; ++i) {
               createMine(x,y);
            }
            cout << "made all mines" << endl;
        }

        void explode() {
            cout << "BIG OL' BOOOOM YOU DIE GG" << endl;
            gameState = 1;
        }

        //clears the current spot and recurses over other completely clear spots 
        void clearSpot(int x, int y) {
            int mineCount = 0;
            std::vector<std::pair<int,int> > todo;

            //check all surrounding squares of the current
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (!isValid(x+i,y+j)) {
                        continue;
                    } else {
                        if (!isSafe(x+i,y+j)) {
                            mineCount += 1;
                        } else {
                            todo.push_back(std::make_pair((x+i),(y+j)));
                        }
                    }
                }
            }
            if (mineCount == 0) {
                cout << "clear spot at: " << x+(y*boardX) << endl;
                boardArray[x+(y*boardX)] = '-';
                //recurse over the adjacent non-mine spaces
                std::vector<std::pair<int,int> >::iterator iter;
                for (iter = todo.begin(); iter != todo.end(); ++iter) {
                        clearSpot(iter->first,iter->second);
                }
            } else {
                boardArray[x+y] = '0' + mineCount;
            }
        }

        void checkWin() {
            int coverCount = 0;
            for (std::vector<int>::size_type i = 0; i != boardArray.size(); ++i) {
                if (boardArray[i] == '@') {
                    ++coverCount;
                }
            }
            if (mineList.size() >= coverCount) {
                gameState = 2;
                cout << "You win" << endl;
            }
        }

    public: 
        board(int x, int y, int num_of_mines) {
            boardArray.reserve(x+y);
            boardX = x;
            boardY = y;

            for (int i = 0; i < (x*y); ++i) {
                boardArray.push_back('@');
            }
            createAllMines(x,y,num_of_mines);
        }
        
        void touchSpot(int x, int y) {
            if (!isSafe(x,y)) {
                explode();
            } else {
                clearSpot(x,y);
            }
            checkWin();
        }

        void displayBoard() {
            int rowNum = 1;
            for (int i = 0; i < boardY ; ++i) {
                cout << (boardY - i - 1);
                if ((boardY - i - 1) < 10) { cout << "  "; } else { cout << " "; };
                for (int j = 0; j < boardX ; ++j) {
                    cout << boardArray[(j*boardX) + i] << "  "; 
                }
                cout << endl;
            }
            cout << " ";
            for (int i = 0; i < boardX; ++i) {
                if (i < 10) { cout << "  ";} else { cout << " ";};
                cout << i;
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
