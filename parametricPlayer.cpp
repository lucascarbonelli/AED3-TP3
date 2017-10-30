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


int scoreBoard(Board &b, int player, vector<int>& weights, ofstream& log){

  size_t feature = 0;
  int score = 0;

  /* Lineas del jugador */
  vector<int> player_lines = b.lineCounts(PLAYER);
  for (size_t i = 0; i < player_lines.size(); i++) {
    log << "Lineas de longitud " << i <<": "<< player_lines[i] << endl;
    score += weights[feature] * player_lines[i];
    feature++;
  }

  /* Perfiles  */
  vector<int> player_profile = b.player_prof(PLAYER);
  for (size_t i = 0; i < player_profile.size(); i++) {
    log << "si muevo a "<< i <<" "<< "puedo hacer una linea de longitud "  << player_profile[i] << endl;
    //score += weights[feature] * player_profile[i];
    feature++;
  }

  vector<int> opponent_profile = b.player_prof(OPPONENT);
  for (size_t i = 0; i < opponent_profile.size(); i++) {
    log << "si el mueve a "<< i <<" "<< "puede hacer una linea de longitud "  << opponent_profile[i] << endl;
    score += weights[feature] * opponent_profile[i];
    feature++;
  }


  /* didIWin? */
  score += weights[feature] * b.didIWin();
  feature++;
  /* didILost? */
  //score += weights[feature] * b.didILost();
  return score;
}





int generateAndScore(Board &b, int player,int c, vector<int>& weights, ofstream& log ){

  int best_move = 0;
  int best_move_score = -INFINITY;

  for (int col = 0; col < b.getColumns(); col++) {
    if( !b.isColFull(col) ){
      /* Miramos el tablero al agregar en la columna col */
      b.addPlayer(col);
      int score = scoreBoard(b,player,weights,log);

      log << "move: " << col << " score: " << score <<endl;
      if(score > best_move_score){
        best_move = col;
        best_move_score = score;
      }
      /* Sacamos la ficha que pusimos */
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
        vector<int> weights(c + 2*columns+1,0);

        for (int i = 0; i < c - 1; i++) {
          weights[i] = exp(i);
        }
        for (int i = c; i < c + columns; i++) {
          weights[i] = exp(i-c+1);
        }
        for (int i = c + columns; i < c + 2*columns ; i++) {
          weights[i] = -exp(i-2*c+5);
        }

        weights[c+2*columns] = INFINITY;
        //weights[3*c] = -INFINITY;

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
