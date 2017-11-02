#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include "genetic.cpp"

std::random_device rd;
std::mt19937 generator(rd());

void send(const std::string& msg) {
    std::cout << msg << std::endl;
}

void send(int msg) {
    std::cout << msg << std::endl;
}

int read_int() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return std::stoi(msg);
}

std::string read_str() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return msg;
}



/* Funfionces propias de genetic player */

int geneticGeneration(int n, int m , int c, int p, bool red_player_first, vector<individual> population){
  
}



int main() {

  int c = 4;
  int columns = 7;
  int rows = 6;
  int p = 21;

  vector<int> weights(c+columns+1,0);
  /*
  for (int i = 0; i < c; i++) {
    weights[i] = i;
  }
  for (int i = c ; i < c + columns; i++) {
    weights[i] = -5*i;
  }*/
  weights = {0, 4, -1, 1, 3, -4, 1, 2, -1, -3, 1, 5};
  weights = {-1, 1, 4, 15, 100, -4, 3, -4, 3, -2, 2, 100};
  vector<int> range{-15,-10,-5,-4,-3,-2,-1,0,1,2,3,4,5,10,15};

  /* fijamos algunas */
  //weights[c-1] = 1000000;
  weights[c+columns] = 1000000;

  weights = randomSearch(c+columns+1,range,150,weights,rows,columns,c,p);

  cout << "Weights: " << endl;
  for (size_t i = 0; i < weights.size(); i++) {
    cout << weights[i] << " ";
  }
  cout << endl;

  float won = fitness(weights,1000,rows,columns,c,p);

    return 0;
}
