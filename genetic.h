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

struct matchResults{
  int won;
  int lost;
  int tied;
  int median_w_time;
  int meadian_l_time;
}

//Ḿétodos

void init_rnd_population(vector<individual>& population, unsigned int max);
void init_population(vector<individual>& population);

void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params);

vector<pair<int, unsigned int> > get_fittest(matchBoard board, vector<individual>& fittest, vector<individual>& population, string player1, string player2, int iter);
void fitness_population(matchBoard board, vector<pait<int, unsigned int> >& scores, vector<individual>& population, string player1, string player2, int iter);
vector<individual> breed_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a);
individual crossover(individual& individual_a, individual& individual_b, int quantInd_a);
void mutation(vector<individual>& population, int prob, int max);

pair<matchResults,matchResults> match(vector<int> weights1,vector<int> weights2, string player1, string player2, int iter, matchBoard& board);

//Auxiliares

//void save_population(vector<individual> population, int n, int m, int c, int p);
bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem);


// Cosas de Pato wip
individual genetic_optimization(matchBoard board, unsigned int pop_size, unsigned int ind_size, unsigned int benchmark, unsigned int max, unsigned int alpha);
void new_generation(vector<individual>& pop);
int rank_population(vector<individual>& pop);
