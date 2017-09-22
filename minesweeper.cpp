#include <iostream>
#include <string>
#include <stdlib.h> // srand, rand
#include <vector>

class board {
    
    private:
        char* boardArray;
       
        std::vector<std::pair<int,int> > posBlackList;

        bool validPos(int x, int y) {
            bool result = true;
            if (x || y) { //check if not null
                std::vector<std::pair<int,int> >::iterator iter;
                for (iter = posBlackList.begin(); iter != posBlackList.end(); ++iter) {
                        if (x == iter->first && y == iter->second) {
                            result = false;
                            break;
                    }
                }
            } else {
                result = false;
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
            
            while (!validPos(randX, randY)) {
                randX = rand() % y;
                randY = rand() % x;
            }
            boardArray[randX][randY] = 'M';
            posBlackList.push_back(std::make_pair(randX,randY));

        }

        void createAllMines(int x, int y, int num_of_mines) {
            for (int i = 0; i < num_of_mines; ++i) {
               createMine(x,y);
            }
        }

    public: 
        board(int x, int y, int num_of_mines) {
            boardArray = new char*[x];
            for (int i = 0; i < x; ++i) {
                boardArray[i] = new char[y];
            }
        }
            
};


int main() {

}
