#include <iostream>
#include <fstream>
#include <stdlib.h>   //rand
#include <utility>    //pair
#include <algorithm>  //sort
#include <math.h> 	  //floor
#include <stdlib.h>   //system
#include <string>
#include <vector>

using namespace std;

typedef vector<int> individual;


struct matchBoard{
  int n; //filas
  int m; //columnas
  int c; //c-linea
  int p; //cantidad fichas
  bool w1_first;
};

struct paramsGen{
  int news;             //cantidad de news para helix
  int breeds;           //cantiadad de better olds a breedear
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
  int median_l_time;
  individual weights;
  int indexPop; //indice en vector<individual> population, si usamos tournament
};

//Ḿétodos

void init_rnd_population(vector<individual>& population, unsigned int max);
void init_population(vector<individual>& population);
individual crossover(individual& individual_a, individual& individual_b, int quantInd_a);
void mutation(vector<individual>& population, int prob, int max);
vector<pair<matchResults,matchResults> > tournament(matchBoard board, vector<individual>& population, int iter);
pair<matchResults,matchResults> match(vector<int> weights1, vector<int> weights2, int p1i, int p2i, int iter, matchBoard& board);

//Helix
void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params, ofstream& log);
vector<pair<int, unsigned int> > get_fittest_helix(matchBoard board, vector<individual> fittest, vector<individual>& population, int iter, ofstream& log);
void fitness_population_helix(vector<pair<matchResults,matchResults> >& tournament_results, vector<pair<int, unsigned int> >& scores);
int score_helix(matchResults match);
vector<individual> breed_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a);


//Paté
individual genetic_optimization(matchBoard board, unsigned int pop_size, unsigned int ind_size, unsigned int benchmark, unsigned int max, unsigned int alpha);
void new_generation(vector<individual>& pop);
int rank_population(vector<individual>& pop);


//Auxiliares

//void save_population(vector<individual> population, int n, int m, int c, int p);
bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem);
int porcentage(int total, int porc);
int mean(vector<vector< int > > numbers, int modifier);