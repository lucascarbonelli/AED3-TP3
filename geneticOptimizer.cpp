#include <iostream>
#include <fstream>
#include <string>
#include <random>
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

    //Ejemplo de ejecución:
    //./geneticOptimizer helix 5 5 4 60 1 10 20 100 3 30 20 2 10000 2 1 1000 ./gen_hel_res.txt ./gen_hel_err.txt

    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    int c = atoi(argv[4]);
    int p = atoi(argv[5]);
    int w1_first = atoi(argv[6]);
    int pop_size = atoi(argv[8]);
    int pop_max_rnd = atoi(argv[9]);
    int best_ones_quant = atoi(argv[10]);
    int iter = atoi(argv[7]);
    int news_porc = atoi(argv[11]);
    int breeds_porc = atoi(argv[12]);
    int cross_fraction = atoi(argv[13]);
    int probMut = atoi(argv[14]);
    int maxMutMod = atoi(argv[15]);
    int test = atoi(argv[16]);
    int matches = atoi(argv[17]);


    string filepath_results = argv[18];
    string filepath_logerror = argv[19];

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
    get_fittest_helix(board, best_ones, population, iter, log_hel_err);
  
    paramsGen params;
    params.news = porcentage(population.size(), news_porc);
    params.breeds = porcentage(population.size(), breeds_porc);
    params.iter = iter;
    params.quantInd_a_Cross = floor((params.news+params.breeds)/cross_fraction);
    params.probMut = probMut;
    params.maxMut = mean(population, maxMutMod);
  
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
  
      get_fittest_helix(board, best_ones, all_individuals, iter, log_hel_err);

    }
    string file_res = filepath_results /*+ encode*/; 
    ofstream log_hel_res(file_res);
    vectorPrinter(best_ones, log_hel_res);

    if(test){
      tester_against_random(best_ones[0], matches, board.n, board.m, board.c, board.c, log_hel_res);
    }

  }

  if(algorithm == "pate"){
    //ALGORITMO PATE

  }
  

  return 0;
}
