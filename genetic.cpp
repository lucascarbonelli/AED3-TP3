#include "genetic.h"


/*---------------------------------------Globales------------------------------------------*/

void init_rnd_population(vector<individual>& population, unsigned int max){
  unsigned int size = population.size();

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < population[0].size(); j++) {
      population[i][j] = rand() % max;
    }
  }
}


void init_population(vector<individual>& population){

//para hacer un init mas inteligente sobre "los mejores" o "los peores"
}


//toma quantInd_a de individual_a para el res, y el resto de _b
//por ejemplo, quantInd_a = individual_a.size()/2 daría mitad de uno y otro
//CUIDADO: toma ints, así que hacer ceil o floor
individual crossover(individual& individual_a, individual& individual_b, int quantInd_a){
  individual res;
  for (int i = 0; i < quantInd_a; ++i){
    res.push_back(individual_a[i]);
  }

  for (int i = quantInd_a; i < individual_b.size(); ++i){
    res.push_back(individual_b[i]);
  }

  return res;
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


//según el enunciado, no es recomendado hacer jugar con minimax_player, tarda mucho
vector<pair<matchResults,matchResults> > tournament(matchBoard board, vector<individual>& population, int iter){

  //en matchResults tenemos qué weight fué
  vector<pair<matchResults,matchResults> > fixture;
  for (int i = 0; i < population.size(); ++i){
    for (int j = i+1; j < population.size(); ++j){
      fixture.push_back(match(population[i], population[j], i, j, iter, board));
    }
  }
  return fixture;
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
  //cmd += " " + to_string(iter); /* cantidad de iteraciones */
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


/*---------------------------------------Selection y fitness Helix------------------------------------------*/


void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params, ofstream& log){
  //tomamos #breeds better ones
  vector<individual> better_ones(params.breeds);
  get_fittest_helix(board, better_ones, population, params.iter, log);

  //nos guardamos los viejos de la población (los no new)
  vector<individual> old_population;
  for (int i = 0; i < population.size()-params.news; ++i){
    old_population.push_back(population[i]);
  }
  //buscamos sus #breeds mejores
  vector<individual> old_better_ones(params.breeds);
  get_fittest_helix(board, old_better_ones, old_population, params.iter, log);

  //reproducimos los viejos mejores con los mejores globales
  vector<individual> new_generation = breed_twopops(old_better_ones, better_ones, params.quantInd_a_Cross);

  //los juntamos en new population
  for (int i = 0; i < params.news; ++i){
    new_population.push_back(population[population.size()-1-i]);
  }
  for (int i = 0; i < params.breeds; ++i){
    new_population.push_back(new_generation[i]);
  }

  //mutamos
  mutation(new_population, params.probMut, params.maxMut);

  //save_population(new_population, n, m, c, p);

}


vector<pair<int, unsigned int> > get_fittest_helix(matchBoard board, vector<individual> fittest, vector<individual>& population, int iter, ofstream& log){

  //first es score y second es indice del individuo en population
  vector<pair<int, unsigned int> > scores(population.size());
  for (int i = 0; i < scores.size(); ++i){scores[i].first = 0;}

  //rankeamos a cada individuo
  vector<pair<matchResults,matchResults> > fixture = tournament(board, population, iter);
  fitness_population_helix(fixture, scores);

  //los ordeno con pairCompare
  sort(scores.begin(), scores.end(), pairCompare);

  //meto todo en fittest
  for (int i = 0; i < fittest.size(); ++i){
    fittest[i] = population[scores[population.size()-1-i].second];
  }

  //devuelvo los scores con su indice en population (si se toma el indice de scores, se ignora scores.second, matchea con fittest)
  return scores;
}


void fitness_population_helix(vector<pair<matchResults,matchResults> >& tournament_results, vector<pair<int, unsigned int> >& scores){

    for (int i = 0; i < tournament_results.size(); ++i){
      scores[tournament_results[i].first.indexPop].first = scores[tournament_results[i].first.indexPop].first + score_helix(tournament_results[i].first);
      scores[tournament_results[i].first.indexPop].second = tournament_results[i].first.indexPop;
      scores[tournament_results[i].second.indexPop].first = scores[tournament_results[i].second.indexPop].first + score_helix(tournament_results[i].second);
      scores[tournament_results[i].second.indexPop].second = tournament_results[i].second.indexPop;
    }
}

int score_helix(matchResults match){
  return (match.won - match.lost + match.tied/2) + (match.median_w_time - match.median_l_time)*2;
}


//este método mezcla los individuos entre dos populations
//puede hacerse otro método que se llame crossover y mezcle los individuos de una sola population
vector<individual> breed_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a){

  vector<individual> res;

  int min_population = population_a.size();
  if(population_b < population_a) min_population = population_b.size();

  for (int i = 0; i < min_population; ++i){
    res.push_back(crossover(population_a[i], population_b[i], quantInd_a));
  }

  return res;
}


/*---------------------------------------Selection y fitness Paté------------------------------------------*/
/*
// Le das un tablero y te genera el mejor jugador
individual genetic_optimization(matchBoard board, unsigned int pop_size, unsigned int ind_size, unsigned int benchmark, unsigned int max, unsigned int alpha){

  // Genero la poblacion inicial
  vector<individual> pop(pop_size,individual(ind_size));
  pop = init_rnd_population(pop,max);

  // Itero hasta superar mi benchmark
  pair<int,unsigned int> best_ind = make_pair(0,0);
  while(best_ind.first < benchmark){

    // Calculo el mejor fitness de la poblacion y los ordeno
    //Esto deberia ordenar la poblacion  de mejor a peor fitness
    // y retorar el mejor score

    best_ind.first = rank_population(pop);

    best_ind.second = pop[0];

    //Genero la nueva poblacion
    new_generation(pop);

  }

  return best_ind.second;

}

int rank_population(vector<individual>& pop){
  return 0;
}

void new_generation(vector<individual>& pop){
  return pop;
}

*/



/*---------------------------------------Auxiliares------------------------------------------*/

/*
//Formato de salida:
//               T·.·_C·_P·/_I·_F·
//Donde en cada punto hay un numero, y la barra es por directorio, y las siglas son:
// T: tamaño del tablero (filas.columnas)
// C: c-linea
// P: cantidad de fichas máxima
// I: cantidad de individuos en la población
// F: cantidad de features
void save_population(vector<individual> population, int n, int m, int c, int p){

  string finalpath = "T" + to_string(n)+"."+to_string(m)+"_C"+to_string(c)+"_P"+to_string(p);
  if (mkdir(finalpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
    //está parte de abajo quizá sea útil...
    /*if( errno == EEXIST ){
        //alredy exists
    } else {
        //something else
        std::cout << "cannot create sessionnamefolder error:" << strerror(errno) << std::endl;
        throw std::runtime_exception( strerror(errno) );
      }
    }//
  }

  string f_name = "training/"+finalpath+"/"+"_I"+to_string(population.size())+"_F"+to_string(population[0].size());
  ofstream file(f_name);

  for (size_t i = 0; i < population.size(); i++) {
    for (size_t j = 0; j < population[i].size(); j++) {
      f_name << population[i][j] << " ";
    }
    f_name << endl;
  }
}*/


bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem){
  return firstElem.first < secondElem.first;
}

int porcentage(int total, int porc){
  return floor(porc*total/100);
}

int mean(vector<vector< int > > numbers, int modifier){
  
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
