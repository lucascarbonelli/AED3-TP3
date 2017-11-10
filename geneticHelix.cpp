#include <iostream>
#include <fstream>
#include <iterator>   //split string
#include <stdlib.h>   //rand
#include <utility>    //pair
#include <algorithm>  //sort, random_shuffle
#include <math.h>     //floor
#include <stdlib.h>   //system
#include <string>
#include <vector>
#include <chrono>     //medir tiempos

#define now std::chrono::high_resolution_clock::now
#define INF std::numeric_limits<int>::max();

using namespace std;

typedef vector<int> individual;


/*------------------------Header------------------------*/

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
  int cross_cut_point; //cuantos genes copiar de a (y por resta, de b)
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
void init_population(vector<individual>& population, string filepath);
void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params, ofstream& log);
vector<pair<int, unsigned int> > get_fittest_helix(matchBoard board, vector<individual>& fittest, vector<individual>& population, string player, int type, ofstream& log);
void fitness_population_helix(vector<pair<matchResults,matchResults> >& tournament_results, vector<pair<int, unsigned int> >& scores, string player, int type);
int score_helix(matchResults match, int type);
vector<individual> breed_helix(vector<individual>& population, int cross_cut_point, ofstream& log);
individual crossover(individual& individual_a, individual& individual_b, int cross_cut_point, ofstream& log);
void mutation(vector<individual>& population, int prob, int min, int max);
vector<pair<matchResults,matchResults> > tournament(matchBoard board, string player, vector<individual>& population);
pair<matchResults,matchResults> match(vector<int> weights1, vector<int> weights2, int p1i, int p2i, string player, matchBoard& board);


//Auxiliares

bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem);
int porcentage(int total, int porc);
int mean(vector<vector< int > >& numbers, int modifier);
void vectorPrinter(vector<vector<int> >& v, ofstream& log);
void miniVectorPrint(vector<int>& v, ofstream& log);
void charPrinter(vector<int>& v, ofstream& log);


/*------------------------Algoritmos------------------------*/


void init_rnd_population(vector<individual>& population, int min, unsigned int max){
  unsigned int size = population.size();

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < population[0].size(); j++) {
      population[i][j] =  min + rand() % max;
    }
  }
}


//este método requiere un archivo de sólo numeros separados por espacios
void init_population(vector<individual>& population, string filepath){

  ifstream file(filepath);
  int size = population[0].size();

  string line;
  int i = 0;
  vector<int> genes;
  while(getline(file, line, ' ') || i < population.size()*size){
    genes.push_back(stoi(line));
    i++;
  }

  int l = 0;
  for (int i = 0; i < population.size(); ++i){
    individual ind;
    for (int j = 0; j < population[0].size(); ++j){
      ind.push_back(genes[l]);
      l++;
    }
    population[i] = ind;
  }

}


void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params, ofstream& log){
  //rankeamos la población, pasamos dummy porque no vamos a guardarnos los mejores globales
  vector<individual> dummy(0);
  vector<pair<int, unsigned int> > scores = get_fittest_helix(board, dummy, population, params.player, params.typeScore, log);

  //buscamos los #breeds, que no sean de los #news, mejores
  vector<individual> old_better_ones;
  int l = population.size()-1;
  while(old_better_ones.size() != params.breeds || l < 0){
    if(scores[l].second < population.size()-1-params.news){
      old_better_ones.push_back(population[scores[l].second]);
    }
    l--;
  }

  //reproducimos estos entre si;
  vector<individual> new_generation = breed_helix(old_better_ones, params.cross_cut_point, log);

  //los mutamos
  mutation(new_generation, params.probMut, params.minMut, params.maxMut);


  //los juntamos en new population
  new_population.clear();
  for (int i = 0; i < params.news; ++i){
    new_population.push_back(population[population.size()-1-i]);
  }
  for (int i = 0; i < params.breeds; ++i){
    new_population.push_back(new_generation[i]);
  }

}


vector<pair<int, unsigned int> > get_fittest_helix(matchBoard board, vector<individual>& fittest, vector<individual>& population, string player, int type, ofstream& log){

  //first es score y second es indice del individuo en population
  vector<pair<int, unsigned int> > scores(population.size());
  for (int i = 0; i < scores.size(); ++i){scores[i].first = 0;}

  //rankeamos a cada individuo
  vector<pair<matchResults,matchResults> > fixture = tournament(board, player, population);

  fitness_population_helix(fixture, scores, player, type);

  //los ordeno con pairCompare
  sort(scores.begin(), scores.end(), pairCompare);

  //meto todo en fittest
  for (int i = 0; i < fittest.size(); ++i){
    fittest[i] = population[scores[population.size()-1-i].second];
  }



  //devuelvo los scores con su indice en population (si se toma el indice de scores, se ignora scores.second, matchea con fittest)
  return scores;
}


void fitness_population_helix(vector<pair<matchResults,matchResults> >& tournament_results, vector<pair<int, unsigned int> >& scores, string player, int type){

    for (int i = 0; i < tournament_results.size(); ++i){
      scores[tournament_results[i].first.indexPop].first = scores[tournament_results[i].first.indexPop].first + score_helix(tournament_results[i].first, type);
      scores[tournament_results[i].first.indexPop].second = tournament_results[i].first.indexPop;
      if(player == "parametric_player"){
      scores[tournament_results[i].second.indexPop].first = scores[tournament_results[i].second.indexPop].first + score_helix(tournament_results[i].second, type);
      scores[tournament_results[i].second.indexPop].second = tournament_results[i].second.indexPop;
      }
    }
}

int score_helix(matchResults match, int type){
  if(type == 1) return match.won - match.lost + match.tied;
  if(type == 2) return match.won/match.median_w_time - match.lost/match.median_l_time;
}


vector<individual> breed_helix(vector<individual>& population, int cross_cut_point, ofstream& log){
  vector<individual> res;
  vector<individual> population_copy = population;
  
  random_shuffle(population_copy.begin(), population_copy.end());

  for (int i = 0; i < population.size(); ++i){
    res.push_back(crossover(population[i], population_copy[i], cross_cut_point, log));
  }

  return res;
}



//toma cross_cut_point de individual_a para el res, y el resto de _b
//por ejemplo, cross_cut_point = individual_a.size()/2 daría mitad de uno y otro
//CUIDADO: toma ints, así que hacer ceil o floor
individual crossover(individual& individual_a, individual& individual_b, int cross_cut_point, ofstream& log){

  individual res;
  for (int i = 0; i < cross_cut_point; ++i){
    res.push_back(individual_a[i]);
  }

  for (int i = cross_cut_point; i < individual_b.size(); ++i){
    res.push_back(individual_b[i]);
  }

  return res;
}


//con una probabilidad 1/prob elegimos un indice al azar y modificamos con un valor al azar entre 0 y max-1
//RECOMENDADO: valores muy grandes para prob
void mutation(vector<individual>& population, int prob, int min, int max){
  for (int i = 0; i < population.size(); ++i){
    //cada individuo tiene 1/prob de chances de mutar de sus caracteristicas
    for (int j = 0; j < population[i].size(); ++j){
      if (rand() % prob == 0){
        unsigned int index = rand() % population[i].size();
        population[i][index] = min + rand() % max;
      }
    }
  }
}


//según el enunciado, no es recomendado hacer jugar con minimax_player, tarda mucho
vector<pair<matchResults,matchResults> > tournament(matchBoard board, string player, vector<individual>& population){

  //en matchResults tenemos qué weight fué
  vector<pair<matchResults,matchResults> > fixture;

  if(player == "parametric_player"){
    for (int i = 0; i < population.size(); ++i){
      for (int j = i+1; j < population.size(); ++j){
        fixture.push_back(match(population[i], population[j], i, j, player, board));
      }
    }
  } else if(player == "minimax_fast"){
    for (int i = 0; i < population.size(); ++i){
      fixture.push_back(match(population[i], population[i], i, i, player, board));
    }
  }
  return fixture;
}

pair<matchResults,matchResults> match(vector<int> weights1, vector<int> weights2, int p1i, int p2i, string player, matchBoard& board){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player";
  /* pasamos weights 1 */
  cmd += " 1"; /* cantidad de iteraciones */
  cmd += " " + to_string(weights1.size()); /* cantidad de parametros */

  for (int i = 0; i < weights1.size(); ++i){cmd += " " + to_string(weights1[i]);}

  if(player == "parametric_player"){
    /* pasamos weights 2*/
    cmd+= " --red_player ./parametric_player";
    cmd += " 1"; /* cantidad de iteraciones */
    cmd += " " + to_string(weights2.size()); /* cantidad de parametros */
    for (int i = 0; i < weights2.size(); ++i){cmd += " " + to_string(weights2[i]);}

  } else if(player == "minimax_fast"){
    cmd += " --red_player ./minimax_alpha_beta_fast_player";
  }

  cmd += " --iterations 1";
  if(board.w1_first){cmd += " --first azul";}
  else {cmd += " --first rojo";}
  cmd += " --columns "+to_string(board.m)+" --rows "+to_string(board.n)+" --p "+to_string(board.p)+" --c "+to_string(board.c)+" ";


  system(cmd.c_str());
  ifstream red_results("rojo.txt");
  ifstream blue_results("azul.txt");

  matchResults blue,red;

  blue_results >> blue.won;
  blue_results >> blue.lost;
  blue_results >> blue.tied;
  blue_results >> blue.median_w_time;
  blue_results >> blue.median_l_time;
  blue.weights = weights1;
  blue.indexPop = p1i;

  red_results >> red.won;
  red_results >> red.lost;
  red_results >> red.tied;
  red_results >> red.median_w_time;
  red_results >> red.median_l_time;
  red.weights = weights2;
  red.indexPop = p2i;

  return make_pair(blue,red);
}



/*---------------------------------------Auxiliares------------------------------------------*/


bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem){
  return firstElem.first < secondElem.first;
}

int porcentage(int total, int porc){
  return floor(porc*total/100);
}

int mean(vector<vector< int > >& numbers, int modifier){
  
  int res = 0;

  for (int i = 0; i < numbers.size(); ++i){
  
    int actual = 0;
    for (int j = 0; j < numbers[0].size(); ++j){
        actual = actual + numbers[i][j];
    }
    actual = actual/numbers[0].size();
    res = res + actual;
  }
  res = res/numbers.size();

  return floor(res*modifier);
}


void vectorPrinter(vector<vector<int> >& v, ofstream& log){
  log << "{";
  for (int i = 0; i < v.size()-1; ++i){
    log << " {";
    for (int j = 0; j < v[i].size()-1; ++j){
      log << v[i][j] << ", ";
    }
    log << v[i][v[i].size()-1] << "}, ";
  }

  log << " {";
  for (int j = 0; j < v[v.size()-1].size()-1; ++j){
    log << v[v.size()-1][j] << ", ";
  }
  log << v[v.size()-1][v[v.size()-1].size()-1] << "} }";
}

void miniVectorPrinter(vector<int>& v, ofstream& log){
  log << '"';
  for (int i = 0; i < v.size()-1; ++i){
    log << v[i] << " ";
  }
  log << v[v.size()-1] << '"';
}

void charPrinter(vector<int>& v, ofstream& log){
  for (int i = 0; i < v.size(); ++i){
    log << v[i] << " ";
  }
}