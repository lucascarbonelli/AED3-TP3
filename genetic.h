#include <iostream>
#include <fstream>
#include <stdlib.h>   //rand
#include <utility>    //pair
#include <algorithm>  //sort, random_shuffle
#include <math.h> 	  //floor
#include <stdlib.h>   //system
#include <string>
#include <vector>
#include <chrono>     //medir tiempos

#define now std::chrono::high_resolution_clock::now
#define INF std::numeric_limits<int>::max();

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
  int quantInd_a_Cross; //cuantos genes copiar de a (y por resta, de b)
  int probMut;          //probabilidad a mutar (será 1/probMut)
  int maxMut;           //maximo valor de mutación
  int minMut;           //mínimo valor de la mutación, para negativos
  int typeScore;        //tipo de cuenta de score a utilizar
  string player;        //player para competir
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

void init_rnd_population(vector<individual>& population, int min, unsigned int max);
void init_population(vector<individual>& population);
void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params, ofstream& log);
vector<pair<int, unsigned int> > get_fittest_helix(matchBoard board, vector<individual>& fittest, vector<individual>& population, string player, int type, ofstream& log);
void fitness_population_helix(vector<pair<matchResults,matchResults> >& tournament_results, vector<pair<int, unsigned int> >& scores, string player, int type);
int score_helix(matchResults match, int type);
vector<individual> breed_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a, ofstream& log);
vector<individual> breed_helix(vector<individual>& population, int quantInd_a, ofstream& log);
individual crossover(individual& individual_a, individual& individual_b, int quantInd_a, ofstream& log);
void mutation(vector<individual>& population, int prob, int min, int max);
vector<pair<matchResults,matchResults> > tournament(matchBoard board, string player, vector<individual>& population);
pair<matchResults,matchResults> match(vector<int> weights1, vector<int> weights2, int p1i, int p2i, string player, matchBoard& board);


//Auxiliares

bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem);
int porcentage(int total, int porc);
int mean(vector<vector< int > >& numbers, int modifier);
void vectorPrinter(vector<vector<int> >& v, ofstream& log);
void miniVectorPrint(vector<int>& v, ofstream& log);