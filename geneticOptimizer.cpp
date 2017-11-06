#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <time.h>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include "genetic.cpp"



pair<float, matchResults> tester_against_fast_minimax(vector<int> weights, int rows , int columns, int c, int p, bool first, int type, int ui, ofstream& log){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player ";
  cmd += " 1"; /* cantidad de iteraciones */
  cmd += " " + to_string(weights.size()); /* cantidad de parametros */

  for (int i = 0; i < weights.size(); ++i){
    cmd += " " + to_string(weights[i]);
  }

  cmd += " --red_player ./minimax_alpha_beta_fast_player";
  cmd += " --iterations 1";
  
  if(first){cmd += " --first azul ";}
  else{cmd+= " --first rojo ";}

  if(ui == 1) cmd += "--ui True";
  cmd +=" --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";

  /* Corremos el juego que dejara los resultados en res.txt*/
  system(cmd.c_str());
  ifstream res("azul.txt");
  matchResults match;
  
  res >> match.won;
  res >> match.lost;
  res >> match.tied;
  res >> match.median_w_time;
  res >> match.median_l_time;

  float score = score_helix(match, type);
  log << endl;
  log << "Score: " << score << endl;
  log << "Won " << match.won << " times."<< endl;
  log << "Lost " << match.lost << " times."<< endl;
  log << "Tied " << match.tied << " times."<< endl;
  
  return make_pair(score, match);
}


pair<float, matchResults> tester_against_random(vector<int> weights, int matches, int rows , int columns, int c, int p, bool first, int type, int ui, ofstream& log){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player ";
  cmd += " " + to_string(matches); /* cantidad de iteraciones */
  cmd += " " + to_string(weights.size()); /* cantidad de parametros */

  for (int i = 0; i < weights.size(); ++i){
    cmd += " " + to_string(weights[i]);
  }

  cmd += " --red_player ./random_player";
  cmd += " --iterations " + to_string(matches);

  if(first){cmd += " --first azul ";}
  else{cmd+= " --first rojo ";}

  if(ui == 1) cmd += "--ui True";
  cmd +=" --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";

  /* Corremos el juego que dejara los resultados en res.txt*/
  system(cmd.c_str());
  ifstream res("azul.txt");
  int won,lost,tied, median_l_time, median_w_time;
  matchResults match;
  
  res >> match.won;
  res >> match.lost;
  res >> match.tied;
  res >> match.median_w_time;
  res >> match.median_l_time;

  float score = score_helix(match, type);
  log << endl;
  log << "Score: " << score << endl;
  log << "Won " << won << " times."<< endl;
  log << "Lost " << lost << " times."<< endl;
  log << "Tied " << tied << " times."<< endl;
  
  return make_pair(score, match);
}






int main(int argc, const char* argv[]) {

  if (argc < 2)
  {
    cout << "Los parámetros de entrada son: " << endl;
    cout << "./geneticOptimizer <tipo de algoritmo genético> <parámetros del algoritmo>" << endl;
    cout << endl;
    cout << "Para saber mas sobre los parámetros del algoritmo elegido," << endl;
    cout << "por favor contacte a la central de servicio técnico de" << endl;
    cout << "Juab's Group® mas cercano a su localidad actual. Gracias." << endl;
    return -1;
  }

  string algorithm = argv[1];

  if(algorithm == "helix"){

    //Ejemplo de ejecución (el null está por la salida de python):
    //./geneticOptimizer helix 7 6 4 60 1 20 150 20 30 20 3 2 100 1 1000 minimax_fast 0 ./gen_hel_test > /dev/null

    /*--------seteo e inicialización de parámetros--------*/
    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    int c = atoi(argv[4]);
    int p = atoi(argv[5]);
    int w1_first = atoi(argv[6]);
    int pop_size = atoi(argv[7]);
    int pop_max_rnd = atoi(argv[8]);
    int pop_min_neg = -1*atoi(argv[9]);
    int news_porc = atoi(argv[10]);
    int breeds_porc = atoi(argv[11]);
    int best_ones_quant = atoi(argv[12]);
    int cross_fraction = atoi(argv[13]);
    int maxMutMod = atoi(argv[14]);
    int typeScore = atoi(argv[15]);
    int matches = atoi(argv[16]);
    string player = argv[17];
    int against = atoi(argv[18]);
    string filepath_logtests = argv[19];

    int ui = 0;
    string filepath_logerror = "delete_this";

    if(argc > 20) ui = atoi(argv[20]);
    if(argc > 22) filepath_logerror = argv[21];

    ofstream log_hel_err(filepath_logerror);
    
    matchBoard board;
    board.m = m;
    board.n = n;
    board.c = c;
    board.p = p;
    board.w1_first = w1_first;
      
    paramsGen params;
    params.news = porcentage(pop_size, news_porc);
    params.breeds = porcentage(pop_size, breeds_porc);
    params.quantInd_a_Cross = floor((board.c-1 + 1 + board.m + board.c-2 + board.c-2)/2);
    params.probMut = pop_size*maxMutMod;
    params.maxMut = pop_size*3;
    params.typeScore = typeScore;
    params.player = player;
    params.minMut = pop_min_neg;
  
    
    srand(time(0));

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;

    t1 = now();
    /*--------comienza el algoritmo--------*/

    vector<individual>  population(pop_size, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
    init_rnd_population(population, pop_min_neg, pop_max_rnd);
    vector<individual> best_ones(best_ones_quant);
    get_fittest_helix(board, best_ones, population, player, typeScore, log_hel_err);

    vector<individual> new_population(params.news+params.breeds, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
    
    while(new_population.size() > 2){
      //corro la genetica
      helix(board, population, new_population, params, log_hel_err);
  
      //population es new population, y new population se vacía y achica
      population.clear();
      population = new_population;
      new_population.clear();
      params.news = porcentage(population.size(), news_porc);
      params.breeds = porcentage(population.size(), breeds_porc);
      new_population.resize(params.news+params.breeds);
      for (int i = 0; i < new_population.size(); ++i){
        individual new_individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2);
        new_population[i] = new_individual;
      }
  
      //actualizo los mejores globales
      vector<individual> all_individuals = population;
      all_individuals.push_back(best_ones[0]);
      all_individuals.push_back(best_ones[1]);
      all_individuals.push_back(best_ones[2]);
  
      get_fittest_helix(board, best_ones, all_individuals, player, typeScore, log_hel_err);

    }
    /*--------termino el algoritmo--------*/
    t2 = now();
    time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2-t1);



    /*--------output de resultados--------*/

     //medidor general de tiempos y scores
     ofstream log_hel_test(filepath_logtests, std::ios_base::app | std::ios_base::out);
     pair<float, matchResults> scoreAndMatch;

     if(against == 0) scoreAndMatch = tester_against_random(best_ones[0], matches, board.n, board.m, board.c, board.p, board.w1_first, typeScore, ui, log_hel_err);
     if(against == 1) scoreAndMatch = tester_against_fast_minimax(best_ones[0], board.n, board.m, board.c, board.p, board.w1_first, typeScore, ui, log_hel_err);

     log_hel_test << endl;
     miniVectorPrinter(best_ones[0], log_hel_test);
     //guardo todo, TODO
     log_hel_test << player << "," << scoreAndMatch.first;
     log_hel_test << "," << scoreAndMatch.second.won << "," << scoreAndMatch.second.lost << "," << scoreAndMatch.second.tied << "," << scoreAndMatch.second.median_w_time << "," << scoreAndMatch.second.median_l_time;
     log_hel_test << "," << pop_size << "," << maxMutMod << "," << best_ones_quant << "," << news_porc << "," << breeds_porc << "," << typeScore;
     log_hel_test << "," << m << "," << n << "," << c << "," << p << "," << time_span.count();


  }

  if(algorithm == "pate"){
    //ALGORITMO PATE

  }
  

  return 0;
}
