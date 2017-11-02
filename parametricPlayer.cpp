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
#include <algorithm>    // std::sort

#define INFINITY std::numeric_limits<int>::max();
using namespace std;


/* Para jugar contra parametric_player :
>> python c_linea.py --blue_player ./parametric_player 10 12 -1 1 4 15 100 -4 3 -4 3 -2 2 1000000 --first azul --ui True --columns 7 --rows 6 --p 21 --c 4

>>python c_linea.py --blue_player ./parametric_player 10 12 0 -100 1000 0 10 -100 1000 -10 50 10 1000 50 --first azul --ui True --columns 7 --rows 6 --p 21 --c 4
0 -100 1000 0 10 -100 1000 -10 50 10 1000 50

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


int scoreBoard(Board &b, int player, vector<int>& weights/*, ofstream& log*/){

  size_t feature = 0;
  int score = 0;

  /* Lineas del jugador */
  vector<int> player_lines = b.lineCounts(PLAYER);
  for (size_t i = 0; i < player_lines.size(); i++) {
    //log << "Lineas de longitud " << i <<": "<< player_lines[i] << endl;
    score += weights[feature] * player_lines[i];
    feature++;
  }

  /* Perfiles  */

  vector<int> opponent_profile = b.player_prof(OPPONENT);
  for (size_t i = 0; i < opponent_profile.size(); i++) {
    //log << "si el mueve a "<< i <<" "<< "puede hacer una linea de longitud "  << opponent_profile[i] << endl;
    score += weights[feature] * opponent_profile[i];
    feature++;
  }


  /* didIWin? */
  score += weights[feature] * b.didIWin();
  return score;
}





int generateAndScore(Board &b, int player,int c, vector<int>& weights/*, ofstream& log*/ ){

  int best_move = 0;
  int best_move_score = -INFINITY;

  for (int col = 0; col < b.getColumns(); col++) {
    if( !b.isColFull(col) ){
      /* Miramos el tablero al agregar en la columna col */
      b.addPlayer(col);
      int score = scoreBoard(b,player,weights);

      //log << "move: " << col << " score: " << score <<endl;
      if(score > best_move_score){
        best_move = col;
        best_move_score = score;
      }
      /* Sacamos la ficha que pusimos */
      b.backtrack(col);
    }
  }
  //log << "Chose " << best_move << " with score " << best_move_score<< endl;
  return best_move;
}



int main(int argc, const char* argv[]) {

    int iters;
    if(argc >= 2){
      iters = atoi(argv[1]);
    }

    vector<int> weights;
    if(argc >= 3){
      int params = atoi(argv[2]);
      for (size_t i = 0; i < params; i++) {
        weights.push_back(atoi(argv[3+i]));
      }
    }

    std::string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;


    /* Guardamos un log de las decision en log.txt */
    ofstream log("log.txt");
    ofstream res("res.txt");
    //log << iters << endl;
    int won = 0, lost = 0, tied = 0;
    bool imBlue = false;
    vector<int> loosing_time;
    vector<int> winning_time;

    while (true) {
        int t = 0;
        color = read_str();
        if(color == "azul") imBlue = true;
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        Board board(columns, rows, c, 2*p); // 2*p fichas en total

        // Primer movimiento
        go_first = read_str();
        if (go_first == "vos"){
          imBlue = true;
          log << "started \n" << endl;
          move = generateAndScore(board,PLAYER,c,weights);
          //log << "move \n" << move <<  endl;
          board.addPlayer(move);
          send(move);
          t++;
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                if(msg == "ganaste"){
                   won++;
                   winning_time.push_back(t);
                 }
                if(msg == "perdiste") {
                   lost++;
                   loosing_time.push_back(t);
                 }
                if(msg == "empataron") tied++;
                break;

            }
            //log << "mi turno " << endl;
            board.addOpponent(std::stoi(msg));
            move = generateAndScore(board,PLAYER,c,weights);
            //log << "move \n" << move <<  endl;
            board.addPlayer(move);
            send(move);
        }
        iters--;
        if (iters == 0 && imBlue) {
          /* mediana del winning time */
          sort(winning_time.begin(), winning_time.end());
          int mean_winning_time = won > 0 ? winning_time[won/2] : 0;

          sort(loosing_time.begin(),loosing_time.end());
          int mean_loosing_time = lost > 0 ? loosing_time[lost/2] : 0;

          res << won << endl << lost << endl << tied << endl;}
    }


    return 0;
}
