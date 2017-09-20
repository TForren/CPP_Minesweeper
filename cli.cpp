#include <iostream>

using namespace std;

int main ( int argc, char *argv[] ){
  cout << argc << " arguments" << endl;
  for(int i=0; i < argc; i++){
    cout << "Arg " << i << ": " << argv[i] << endl;
  }
  return 0;
}