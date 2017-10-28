#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <limits>
#include "board.h"
#include <vector>

#define INFINITY std::numeric_limits<int>::max();
using namespace std;


/* Para jugar contra parametric_player :
>> python c_linea.py --blue_player ./parametric_player --first azul --ui True --columns 7 --rows 6 --p 21 --c 4
*/


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

/* Template de la funcion  */

int generateAndScore(Board &b, unsigned int player,int c, vector<int>& weights, ofstream& log ){

  int best_move = 0;
  int best_move_score = -INFINITY;

  for (int col = 0; col < b.getColumns(); col++) {
    if( !b.isColFull(col) ){
      /* Miramos el tablero al agregar en la columna col */
      b.addPlayer(col);
      int score = 0;
      for (int l = 2; l <= c; l++) {
        log << "found " << b.linesOfSize(l,PLAYER) << "lines of size " << l << endl;
        score += b.linesOfSize(l,PLAYER) * weights[l-1];
      }
      log << "move: " << col << " score: " << score <<endl;
      if(score > best_move_score){
        best_move = col;
        best_move_score = score;
      }
      /* Sacamos la ficha que pusimo */
      b.backtrack(col);
    }
  }
  log << "Chose " << best_move << " with score " << best_move_score<< endl;
  return best_move;
}



int main() {

    std::string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

    /* Guardamos un log de las decision en log.txt */
    ofstream log("log.txt");


    while (true) {
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        Board board(columns, rows, c, 2*p); // 2*p fichas en total
        /* Inicializamos los pesos */
        vector<int> weights(2*c,0);
        for (int i = 0; i < c; i++) {
          weights[i] = i+1;
        }
        weights[c] = INFINITY;

        // Primer movimiento
        go_first = read_str();
        if (go_first == "vos"){
          log << "started \n" << endl;
          move = generateAndScore(board,PLAYER,c,weights,log);
          log << "move \n" << move <<  endl;
          board.addPlayer(move);
          send(move);
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }
            log << "mi turno " << endl;
            board.addOpponent(std::stoi(msg));
            move = generateAndScore(board,PLAYER,c,weights,log);
            log << "move \n" << move <<  endl;
            board.addPlayer(move);
            send(move);

        }
    }

    return 0;
}
