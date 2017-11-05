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



float tester_against_random(vector<int> weights, int matches, int rows , int columns, int c, int p, ofstream& log){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player ";
  cmd += " " + to_string(matches); /* cantidad de iteraciones */
  cmd += " " + to_string(weights.size()); /* cantidad de parametros */

  for (int i = 0; i < weights.size(); ++i){
    cmd += " " + to_string(weights[i]);
  }

  cmd += " --red_player ./random_player";
  cmd += " --iterations " + to_string(matches);
  cmd += " --first azul --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";

  /* Corremos el juego que dejara los resultados en res.txt*/
  system(cmd.c_str());
  ifstream res("azul.txt");
  int won,lost,tied, median_l_time, median_w_time;
  res >> won;
  res >> lost;
  res >> tied;
  res >> median_w_time;
  res >> median_l_time;
  //cout << "Ganados: "<< won << " | " << "Perdidos: " << lost << " | " << "Empatados: " << tied <<endl;
  //cout << "median_w_time: " << median_w_time << " | " << "median_l_time: " << median_l_time << endl;

  float score = won/median_w_time - lost/median_l_time;
  log << endl;
  log << "Score: " << score << endl;
  log << "Won " << won << " times."<< endl;
  log << "Lost " << lost << " times."<< endl;
  log << "Tied " << tied << " times."<< endl;
  return score;

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
    //./geneticOptimizer helix 7 6 4 60 1 5 20 100 3 30 20 2 10000 2 1 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null

    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    int c = atoi(argv[4]);
    int p = atoi(argv[5]);
    int w1_first = atoi(argv[6]);
    int iter = atoi(argv[7]);
    int pop_size = atoi(argv[8]);
    int pop_max_rnd = atoi(argv[9]);
    int best_ones_quant = atoi(argv[10]);
    int news_porc = atoi(argv[11]);
    int breeds_porc = atoi(argv[12]);
    int cross_fraction = atoi(argv[13]);
    int probMut = atoi(argv[14]);
    int maxMutMod = atoi(argv[15]);
    int type = atoi(argv[16]);
    int outputMode = atoi(argv[17]);
    int matches = atoi(argv[18]);


    string filepath_results = argv[19];
    string filepath_logerror = argv[20];
    string filepath_logtests = argv[21];

    ofstream log_hel_err(filepath_logerror);
  
    matchBoard board;
    board.m = m;
    board.n = n;
    board.c = c;
    board.p = p;
    board.w1_first = w1_first;
  
    vector<individual>  population(pop_size, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
    init_rnd_population(population, pop_max_rnd);

    vector<individual> best_ones(best_ones_quant);
    get_fittest_helix(board, best_ones, population, iter, type, log_hel_err);
  
    paramsGen params;
    params.news = porcentage(population.size(), news_porc);
    params.breeds = porcentage(population.size(), breeds_porc);
    params.iter = iter;
    params.quantInd_a_Cross = floor((board.c-1 + 1 + board.m + board.c-2 + board.c-2)/2);
    params.probMut = probMut;
    params.maxMut = pop_max_rnd*maxMutMod;
    params.typeScore = type;
  
    vector<individual> new_population(params.news+params.breeds, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
    
    srand(time(0));

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;

    t1 = now();
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
  
      get_fittest_helix(board, best_ones, all_individuals, iter, type, log_hel_err);

    }
    t2 = now();
    time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2-t1);

    string file_res = filepath_results /*+ encode*/; 
    ofstream log_hel_res(file_res);
    int score;

    //quizá cambiar esto por switch
    if(outputMode == 1){
      //imprimir score del mejor contra random
      miniVectorPrinter(best_ones[0], log_hel_res);
      tester_against_random(best_ones[0], matches, board.n, board.m, board.c, board.c, log_hel_res);
    }
    if(outputMode == 2){
      //imprimir todos los mejores
      vectorPrinter(best_ones, log_hel_res);
    }
    if(outputMode == 3){
      //medidor general de tiempos y scores
      ofstream res_weights;
      res_weights.open(filepath_logtests, std::ios::in | std::ios::out | std::ios::ate);

      score = tester_against_random(best_ones[0], matches, board.n, board.m, board.c, board.c, log_hel_res);
      
      res_weights << endl;
      miniVectorPrinter(best_ones[0], res_weights);
      res_weights << "," << score << "," << pop_size << "," << pop_max_rnd << "," << best_ones_quant << "," << news_porc << "," << breeds_porc << "," << iter << "," << time_span.count();

    }

  }

  if(algorithm == "pate"){
    //ALGORITMO PATE

  }
  

  return 0;
}
