#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

#include <fstream>
#include <stdlib.h>   //rand
#include <utility>    //pair
#include <algorithm>  //sort, random_shuffle
#include <math.h> 	  //floor
#include <string>

using namespace std;

typedef vector<int> individual;

struct matchBoard{
  int n; //filas
  int m; //columnas
  int c; //c-linea
  int p; //cantidad fichas
  bool w1_first;
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

void uIntFloatPairPrinter(vector<pair<unsigned int,float> > v){
  cout << "size: " << v.size()<< ", ";
  cout << "{";
  for (int i = 0; i < v.size()-1; ++i)
  {
    cout << "(" << v[i].first <<"-"<< v[i].second<<")" << "," ;
  }
  cout <<"(" << v[v.size()-1].first <<"-"<< v[v.size()-1].second << ")" << "}"<< endl;
  return;
}

void popPrinter(vector<individual>& v){
  cout << "{";
  for (int i = 0; i < v.size()-1; ++i){
    cout << " {";
    for (int j = 0; j < v[i].size()-1; ++j){
      cout << v[i][j] << ", ";
    }
    cout << v[i][v[i].size()-1] << "}, ";
  }

  cout << " {";
  for (int j = 0; j < v[v.size()-1].size()-1; ++j){
    cout << v[v.size()-1][j] << ", ";
  }
  cout << v[v.size()-1][v[v.size()-1].size()-1] << "} }";
}



void uIntVectorPrinter(vector<unsigned int>& v){
  cout << "size: " << v.size()<< ", ";
  cout << "{";
  for (int i = 0; i < v.size()-1; ++i)
  {
    cout << v[i] << "," ;
  }
  cout << v[v.size()-1] << "}" <<endl;
  return;
}

void init_rnd_population(vector<individual>& population, unsigned int max){
  unsigned int size = population.size();

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < population[0].size(); j++) {
      population[i][j] = -500 + rand() % 150;
    }
  }
}


//con una probabilidad 1/prob elegimos un indice al azar y modificamos con un valor al azar entre 0 y max-1
//RECOMENDADO: valores muy grandes para prob
void mutation(vector<individual>& population, int prob, int max){
  for (int i = 0; i < population.size(); ++i){
    //cada individuo tiene 1/prob de chances de mutar de sus caracteristicas
    for (int j = 0; j < population[i].size(); ++j){
      if (rand() % prob == 0){
        unsigned int index = rand() % population[i].size();
        population[i][index] = rand() % max;
      }
    }
  }
}

pair<matchResults,matchResults> match(vector<int> weights1, vector<int> weights2, int p1i, int p2i, int iter, matchBoard& board){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player";
  /* pasamos weights 1 */
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights1.size()); /* cantidad de parametros */

  for (int i = 0; i < weights1.size(); ++i){
    cmd += " " + to_string(weights1[i]);
  }

  /* pasamos weights 2*/
  cmd+= " --red_player ./parametric_player";
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights2.size()); /* cantidad de parametros */

  for (int i = 0; i < weights2.size(); ++i){
    cmd += " " + to_string(weights2[i]);
  }

  cmd += " --iterations " + to_string(iter);
  if(board.w1_first) {
    cmd += " --first azul --columns "+to_string(board.m)+" --rows "+to_string(board.n)+" --p "+to_string(board.p)+" --c "+to_string(board.c)+" ";
  } else {
    cmd += " --first rojo --columns "+to_string(board.m)+" --rows "+to_string(board.n)+" --p "+to_string(board.p)+" --c "+to_string(board.c)+" ";
  }


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


matchResults match_boss(vector<int> weights1, int p1i, matchBoard& board){

  /* Seteamos parametros */
  string cmd = "python2 c_linea.py --blue_player ./parametric_player";
  /* pasamos weights 1 */
  cmd += " " + to_string(1); /* cantidad de iteraciones */
  cmd += " " + to_string(weights1.size()); /* cantidad de parametros */

  for (int i = 0; i < weights1.size(); ++i){
    cmd += " " + to_string(weights1[i]);
  }

  cmd += " --red_player ./minimax_alpha_beta_fast_player 1";
  cmd += " --iterations 1";

  if(board.w1_first) {
    cmd += " --first azul --columns "+to_string(board.m)+" --rows "+to_string(board.n)+" --p "+to_string(board.p)+" --c "+to_string(board.c)+" ";
  } else {
    cmd += " --first rojo --columns "+to_string(board.m)+" --rows "+to_string(board.n)+" --p "+to_string(board.p)+" --c "+to_string(board.c)+" ";
  }


  system(cmd.c_str());
  ifstream blue_results("azul.txt");

  matchResults blue;

  blue_results >> blue.won;
  blue_results >> blue.lost;
  blue_results >> blue.tied;
  blue_results >> blue.median_w_time;
  blue_results >> blue.median_l_time;
  blue.weights = weights1;
  blue.indexPop = p1i;

  return blue;
}


//según el enunciado, no es recomendado hacer jugar con minimax_player, tarda mucho
vector<pair<unsigned int,float > > tournament(matchBoard board, vector<individual>& population, int iter){

  //en matchResults tenemos qué weight fué
  vector<pair<unsigned int,pair<unsigned int, unsigned int> > > scores;
  for (size_t i = 0; i < population.size(); i++) {
    scores.push_back(make_pair(i,make_pair(0,0)));
  }

  for (int i = 0; i < population.size(); ++i){
    for (int j = i+1; j < population.size(); ++j){
      pair<matchResults,matchResults> matchRes = match(population[i], population[j], i, j, iter, board);
      scores[i].second.first += matchRes.first.won;
      scores[i].second.second += matchRes.first.lost + matchRes.first.won + matchRes.first.tied;
      scores[j].second.first += matchRes.first.won;
      scores[j].second.second += matchRes.second.lost + matchRes.second.won + matchRes.second.tied;
      
    }
    matchResults matchRes = match_boss(population[i], i, board);
    //cout << matchRes.won << "," << matchRes.lost <<"," << matchRes.tied << endl;
    scores[i].second.first += matchRes.won;
    scores[i].second.second += matchRes.lost + matchRes.won + matchRes.tied;

  }
  vector<pair<unsigned int,float> > res;
  for (size_t i = 0; i < scores.size(); i++) {
    cout << scores[i].second.first << " ---- " << scores[i].second.second << endl;;
    float rank = float(scores[i].second.first)/float(scores[i].second.second);
    //cout << rank << endl;
    res.push_back(make_pair(i,rank));
  }

  return res;
}

bool matchCompare(const pair<unsigned int,float>& firstElem, const pair<unsigned int,float>& secondElem){
  return firstElem.second > secondElem.second;
}



float rank_population(vector<pair<unsigned int,float> >& scores, vector<unsigned int>& ranking){
  sort(scores.begin(), scores.end(), matchCompare);
  for (size_t i = 0; i < scores.size(); i++) {
    ranking.push_back(scores[i].first);
  }
  return scores[0].second;
}

pair<unsigned int, unsigned int> select(vector<unsigned int> ranking, vector<individual> population){
  random_device rd;
  mt19937 gen(rd());
  binomial_distribution<> d(population.size(), 0.1);
  pair<unsigned int, unsigned int> parents = make_pair(d(gen),d(gen));
  return parents;
}


individual crossover(individual father1, individual father2, unsigned int cut_point){
  individual child;
  for (size_t i = 0; i < cut_point; i++) {
    child.push_back(father1[i]);
  }
  for (size_t i = cut_point; i < father2.size(); i++) {
    child.push_back(father2[i]);
  }
  return child;
}

individual breed(individual father1, individual father2){
  unsigned int crossover_cut_point = rand() % father1.size();
  individual child = crossover(father1,father2,crossover_cut_point);
  return child;
}

void mutate(individual& ind){
  int MAX = 150;
  random_device rd;
  mt19937 gen(rd());
  // give "true" 1/5 of the time
  // give "false" 4/5 of the time
  bernoulli_distribution d(0.2);

  for (size_t i = 0; i < ind.size(); i++) {
    if(d(gen)) ind[i] = -50 + (rand() % MAX);
  }
}

vector<individual> new_generation(vector<unsigned int>& ranking, vector<individual> population, unsigned int population_size){

  vector<individual> new_population;
  // Replico los n**1/2 mejores individuos
  for (int i = 0; i < ceil(sqrt(population_size)); ++i){
    individual sqrt_best = population[ranking[i]];
    new_population.push_back(sqrt_best);
  }


  while(new_population.size() < population_size ){
    pair<unsigned int,unsigned int> parents = select(ranking,population);
    //cout << "cogiendo..." << endl;
    individual child = breed(population[parents.first], population[parents.second]);
    //cout << "mutando..." << endl;
    mutate(child);
    //cout << "agregando... " << endl;
    new_population.push_back(child);
  }
  return new_population;
}





vector<individual> genetic_optimization(matchBoard board, unsigned int pop_size, unsigned int ind_size, unsigned int max_generations, unsigned int max, unsigned int iter, float benchmark, int min_gen){

  int generation = 0;

  vector<individual> population(pop_size,individual(ind_size));
  srand(time(0));
  init_rnd_population(population,max);
  //popPrinter(population);
  cout << "Genero poblacion inicial" << endl;;
  //// Itero hasta superar mi benchmark
  float best_ind = 0;
  while(min_gen > generation || generation < max_generations && best_ind < benchmark){
    cout << "Generation: " << generation << endl;
    // Calculo el mejor fitness de la poblacion y los ordeno
    //Esto deberia ordenar la poblacion  de mejor a peor fitness
    // y retorar el mejor score
    cout << "Compitiendo..." << endl;
    vector<pair<unsigned int,float> > fitness = tournament(board,population,iter);
    cout << "Rankeando..." << endl;
    //uIntFloatPairPrinter(fitness);
    vector<unsigned int> ranking;
    best_ind = rank_population(fitness,ranking);
    uIntVectorPrinter(ranking);
    //uIntVectorPrinter(ranking);
    cout << "Bredeando..." << endl;
    population = new_generation(ranking,population,population.size());
    generation++;
    cout << "Mejor score: " << best_ind << endl;
  }

  return population;

}


int main(){
  srand(time(0));

  matchBoard board;
  board.n = 6;
  board.m = 7;
  board.c = 4;
  board.p = 21;
  board.w1_first = true;
  int iter = 10;
  int min_gen = 5;
  vector<individual> population = genetic_optimization(board,50,16,100,100,iter,0.99,min_gen);

  return 0;
}





















