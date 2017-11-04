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

  ofstream log("log_gen_helix.txt");

  log << "comenzo el debug" << endl;

  matchBoard board;
  board.m = 5;
  board.n = 5;
  board.c = 4;
  board.p = 60;
  board.w1_first = true;

  log << "board inicial" << endl;
 
  vector<individual>  population(20, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));
  init_rnd_population(population, 100);

  log << "population inicial" << endl;

  vector<individual> best_ones(3);
  get_fittest_helix(board, best_ones, population, 10, log);

  log << "fittest helix inicial" << endl;

  paramsGen params;
  params.news = porcentage(population.size(), 30);
  params.breeds = porcentage(population.size(), 20);
  params.iter = 10;
  params.quantInd_a_Cross = floor((params.news+params.breeds)/2);
  params.probMut = 100000;
  params.maxMut = mean(population, 2.5);

  log << "params inicial" << endl;

  vector<individual> new_population(params.news+params.breeds, individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2));

  log << "new population inicial" << endl;

  log << endl << "empieza ciclo" << endl;

  while(new_population.size() > 2){
    //corro la genetica
    log <<  endl << "con población: " << population.size() << endl;
    helix(board, population, new_population, params, log);
    log << "termine helix" << endl;
    //population es new population, y new population se vacía y achica
    log << population.size() << endl;
    population.clear();
    log << population.size() << endl;
    population = new_population;
    log << "MIRAR ACAAAAAA: " << new_population.size() << " " << population.size() << endl;
    new_population.clear();
    params.news = porcentage(population.size(), 30);
    params.breeds = porcentage(population.size(), 20);
    new_population.resize(params.news+params.breeds);
    for (int i = 0; i < new_population.size(); ++i){
      individual new_individual(board.c-1 + 1 + board.m + board.c-2 + board.c-2);
      new_population[i] = new_individual;
    }

    log << "actualize todo" << endl;

    //actualizo los mejores globales
    vector<individual> all_individuals = population;
    all_individuals.push_back(best_ones[0]);
    all_individuals.push_back(best_ones[1]);
    all_individuals.push_back(best_ones[2]);

    log << "forme all individuals" << endl;

    get_fittest_helix(board, best_ones, all_individuals, 10, log);   

    log << "termine está iteración" << endl;
  }

  ofstream logRes("res.txt");
  vectorPrinter(best_ones, logRes);

  return 0;
}
