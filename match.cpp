#include <iostream>
#include <fstream>
#include <stdlib.h>   //rand
#include <utility>    //pair
#include <algorithm>  //sort, random_shuffle
#include <math.h>     //floor
#include <stdlib.h>   //system
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>
#include <time.h>
#include <chrono>     //medir tiempos


#define now std::chrono::high_resolution_clock::now
#define INF std::numeric_limits<int>::max();

using namespace std;

struct matchBoard{
  int n; //filas
  int m; //columnas
  int c; //c-linea
  int p; //cantidad fichas
  bool w1_first;
};

struct matchResults{
  int won;
  int lost;
  int tied;
  int median_w_time;
  int median_l_time;
  vector<int> weights;
  int indexPop; //indice en vector<individual> population, si usamos tournament (aca no se usa)
};


pair<matchResults,matchResults> finalMatch(vector<int> weights1, vector<int> weights2, int ui, int matches, string player, matchBoard& board){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player";
  /* pasamos weights 1 */
  cmd += " 1"; /* cantidad de iteraciones */
  cmd += " " + to_string(weights1.size()); /* cantidad de parametros */

  for (int i = 0; i < weights1.size(); ++i){cmd += " " + to_string(weights1[i]);}

  if(player == "parametric_player"){
    /* pasamos weights 2*/
    cmd+= " --red_player ./parametric_player";
    cmd += " 1"; /* cantidad de iteraciones */
    cmd += " " + to_string(weights2.size()); /* cantidad de parametros */
    for (int i = 0; i < weights2.size(); ++i){cmd += " " + to_string(weights2[i]);}

  } else if(player == "minimax_alpha_beta_fast_player"){
    cmd += " --red_player ./minimax_alpha_beta_fast_player";
    cmd += " --iterations 1";
  }

  if(player == "random_player"){
  	cmd += " --red_player ./random_player";
	cmd += " --iterations " + to_string(matches);
  }
  if(board.w1_first){cmd += " --first azul";}
  else {cmd += " --first rojo";}
  cmd += " --columns "+to_string(board.m)+" --rows "+to_string(board.n)+" --p "+to_string(board.p)+" --c "+to_string(board.c)+" ";

  if(ui) cmd += "--ui true";

  system(cmd.c_str());
  ifstream red_results("rojo.txt");
  ifstream blue_results("azul.txt");

  matchResults blue,red;

  blue_results >> blue.won;
  blue_results >> blue.lost;
  blue_results >> blue.tied;
  blue_results >> blue.median_w_time;
  blue_results >> blue.median_l_time;
  blue.weights = weights1;

  red_results >> red.won;
  red_results >> red.lost;
  red_results >> red.tied;
  red_results >> red.median_w_time;
  red_results >> red.median_l_time;
  red.weights = weights2;

  return make_pair(blue,red);
}


void miniVectorPrinter(vector<int>& v, ofstream& log){
  log << "{";
  for (int i = 0; i < v.size()-1; ++i){
    log << v[i] << ",";
  }
  log << v[v.size()-1] << "}";
}



int main(int argc, const char* argv[]){
  //./finalMatch weights.size() parametric_player 1 1 7 6 4 60 1 todaLasWeights1 todasLasWeights2
  //./finalMatch 14 parametric_player 1 1 7 6 4 60 1 74 26 22 45 11 -17 -12 21 108 46 81 81 27 128 4 15 -10 0 1 2 3 2 1 0 5 10 -6 -11

  int features = atoi(argv[1]);
  string second_player = argv[2];
  int matches = atoi(argv[3]);
  int ui = atoi(argv[4]);

  matchBoard board;
  board.m = atoi(argv[5]);
  board.n = atoi(argv[6]);
  board.c = atoi(argv[7]);
  board.p = atoi(argv[8]);
  board.w1_first = false;
  if(atoi(argv[9])) board.w1_first = true;
  
  vector<int> weights1;
  vector<int> weights2;
  string player;
 
  for (int i = 0; i < features; i++){
    weights1.push_back(atoi(argv[10+i]));
    if(second_player == "parametric_player") weights2.push_back(atoi(argv[10+features+i]));
  }

  pair<matchResults,matchResults> match = finalMatch(weights1, weights2, matches, player, board);

  ofstream log("archivo.txt", std::ios_base::app | std::ios_base::out);
  miniVectorPrinter(weights1, log);
  log << endl;
  log << "Won " << match.first.won << " times."<< endl;
  log << "Lost " << match.first.lost << " times."<< endl;
  log << "Tied " << match.first.tied << " times."<< endl;

  miniVectorPrinter(weights2, log);
  log << endl;
  log << "Won " << match.second.won << " times."<< endl;
  log << "Lost " << match.second.lost << " times."<< endl;
  log << "Tied " << match.second.tied << " times."<< endl;

	return 0;
}