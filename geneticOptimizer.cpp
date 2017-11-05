#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include "genetic.cpp"


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

    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    int c = atoi(argv[4]);
    int p = atoi(argv[5]);
    bool w1_first = false;
    if(argv[6] == "true") w1_first = true;
    int pop_size = atoi(argv[8]);
    int pop_max_rnd = atoi(argv[9]);
    int best_ones_quant = atoi(argv[10]);
    int iter = atoi(argv[7]);
    int news_porc = atoi(argv[11]);
    int breeds_porc = atoi(argv[12]);
    int probMut = atoi(argv[13]);
    int maxMutMod = atoi(argv[14]);

    string filepath_results = argv[15];
    string filepath_logerror = argv[16];

    ofstream log(filepath_logerror);
  
    matchBoard board;
    board.m = m;
    board.n = n;
    board.c = c;
    board.p = p;
    board.w1_first = w1_first;
  
    vector<individual>  population(pop_size, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
    init_rnd_population(population, pop_max_rnd);

    vector<individual> best_ones(best_ones_quant);
    get_fittest_helix(board, best_ones, population, iter, log);
  
    paramsGen params;
    params.news = porcentage(population.size(), news_porc);
    params.breeds = porcentage(population.size(), breeds_porc);
    params.iter = iter;
    params.quantInd_a_Cross = floor((params.news+params.breeds)/2);
    params.probMut = probMut;
    params.maxMut = mean(population, maxMutMod);
  
    vector<individual> new_population(params.news+params.breeds, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
  
    while(new_population.size() > 3){
      //corro la genetica
      helix(board, population, new_population, params, log);
  
      //population es new population, y new population se vacía y achica
      population.clear();
      population = new_population;
      new_population.clear();
      params.news = porcentage(population.size(), 30);
      params.breeds = porcentage(population.size(), 20);
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
  
      get_fittest_helix(board, best_ones, all_individuals, 10, log);   
      
      string file_res = filepath_results /*+ encode*/; 
      ofstream logRes(file_res);
      vectorPrinter(best_ones, logRes);
 
    }


    if(algorithm == "pate"){
      //ALGORITMO PATE

    }
  
  }
  return 0;
}
