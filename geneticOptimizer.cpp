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
    cout << "Los parametros de entrada son: " << endl;
    cout << "./geneticOptimizer <tipo de algoritmo genético> <parametros del algoritmo>" << endl;
    cout << endl;
    cout << "Para saber mas sobre los parametros del algoritmo elegido," << endl;
    cout << "por favor contacte a la central de servicio ténico de" << endl;
    cout << "Juab's Group® mas cercano a su localidad actual. Gracias." << endl;
    return -1;
  }


  ofstream log("log_gen_helix.txt");

  matchBoard board;
  board.m = 5;
  board.n = 5;
  board.c = 4;
  board.p = 60;
  board.w1_first = true;

  vector<individual>  population(20, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
  init_rnd_population(population, 100);

  vector<individual> best_ones(3);
  get_fittest_helix(board, best_ones, population, 10, log);

  paramsGen params;
  params.news = porcentage(population.size(), 30);
  params.breeds = porcentage(population.size(), 20);
  params.iter = 10;
  params.quantInd_a_Cross = floor((params.news+params.breeds)/2);
  params.probMut = 100000;
  params.maxMut = mean(population, 2.5);

  vector<individual> new_population(params.news+params.breeds, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));

  while(new_population.size() > 2){
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

  }

  ofstream logRes("gen_helix_res.txt");
  vectorPrinter(best_ones, logRes);

  return 0;
}
