#include <iostream>
#include <fstream>
#include <stdlib.h>   //rand
#include <utility>    //pair
#include <algorithm>  //sort
#include <vector>

using namespace std;

typedef vector<int> individual;


struct matchBoard{
  int n; //filas
  int m; //columnas
  int c; //c-linea
  int p; //cantidad fichas
  bool red_player_first;
};

struct paramsGen{
  int news;             //cantidad de news para helix
  int breeds;           //cantiadad de better olds a breedear
  string fitness;       //qué fitness usar
  int iter;             //cuantas iteracones para el fitness others
  int quantInd_a_Cross; //cuantos genes copiar de a (y por resta, de b)
  int probMut;          //probabilidad a mutar (será 1/probMut)
  int maxMut;           //maximo valor de mutación
};

//Ḿétodos

void init_rnd_population(vector<individual>& population, unsigned int max);
void init_population(vector<individual>& population);

void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params);

void get_better_ones(matchBoard board, vector<individual>& better_ones, vector<individual>& population, string fitness, int iter);
vector<pair<int, unsigned int> > fitness_population(matchBoard board, vector<individual>& population);
vector<pair<int, unsigned int> > fitness_others(matchBoard board, vector<individual>& population, int iter);
vector<individual> crossover_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a);
individual breed(individual& individual_a, individual& individual_b, int quantInd_a);
void mutation(vector<individual>& population, int prob, int max);

//Auxiliares

//void save_population(vector<individual> population, int n, int m, int c, int p);
bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem);