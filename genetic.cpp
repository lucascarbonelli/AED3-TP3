#include "genetic.h"


/******************************************************************************/
/******************************************************************************/




/******************************************************************************/
/******************************************************************************/


/*
vector<pair<int,unsigned int> > tournament(vector<individual> population, matchBoard& board){
  // vector de pares que representan < num_inidividio , score_total >

  int NUM_MATCHES = 3; // Numero de partidas que juegan 2 individos

  vector<pair<int,unsigned int> > results(make_pair(0,0));
  for (size_t i = 0; i < population.size(); i++) {
    results[i] = i;
    for (size_t j = 0; j < population.size(); j++) {
      if(i != j){
        pair<float,float> result = match(population[i],population[j],NUM_MATCHES,board);
        results[i].second += (int) result.first;
        results[j].second += (int) result.second;
      }
    }
  }
  return results;
}

*/
void init_rnd_population(vector<individual>& population, unsigned int max){
  unsigned int size = population.size();
  unsigned int num_features = population[0].size();

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < num_features; j++) {
      population[i][j] = rand() % max;hile(best_ind.first < benchmark){

    // Calculo el mejor fitness de la poblacion y los ordeno
    //Esto deberia ordenar la poblacion  de mejor a peor fitness
    // y retorar el mejor score

    best_ind.first = rank_population(pop);

    best_ind.second = pop[0];

    }
  }

  }
}


void init_population(vector<individual>& population){

//para hacer un init mas inteligente sobre "los mejores" o "los peores"
}



void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params){

  vector<individual> better_ones(params.crossovers);
  get_fittest(board, better_ones, population, params.fitness, params.iter);

  vector<individual> old_population;
  for (int i = 0; i < population.size()-params.news; ++i){
    old_population.push_back(population[i]);
  }
  vector<individual> old_better_ones(params.crossovers);
  get_fittest(board, old_better_ones, old_population, params.fitness, params.iter);

  old_better_ones = breed_twopops(old_better_ones, better_ones, params.quantInd_a_Cross);


  for (int i = 0; i < params.news; ++i){
    new_population.push_back(population[population.size()-1-i]);
  }
  for (int i = 0; i < params.crossovers; ++i){
    new_population.push_back(old_better_ones[i]);
  }

  mutation(new_population, params.probMut, params.maxMut);

  //save_population(new_population, n, m, c, p);

}

//copia a fittest los fittest.size() mejores según fitness (notar que hay que pasar vector inicializado con tamaño)
vector<pair<int, unsigned int> > get_fittest(matchBoard board, vector<individual>& fittest, vector<individual>& population, string player1, string player2, int iter){

  //first es score y second es indice del individuo en population
  vector<pair<int, unsigned int> > scores(population.size());

  //rankeamos a cada individuo
  fitness_population(board, scores, population, player1, player2, iter);

  //los ordeno con pairCompare
  sort(scores.begin(), scores.end(), pairCompare);

  //meto todo en fittest
  for (int i = 0; i < fittest.size(); ++i){
    fittest[i] = population[scores[population.size()-1-i].second];
  }

  //devuelvo los scores con su indice en population (si se toma el indice de scores, se ignora scores.second, matchea con fittest)
  return scores;
}


vector<pair<matchResults,matchResults> > fitness_population(matchBoard board, vector<pait<int, unsigned int> >& scores, vector<individual>& population, string player1, string player2, int iter){

  vector<pair<matchResults,matchResults> > fixture;
  for (int i = 0; i < population.size(); ++i){
    //si es minimax_player, minimax no necesita weights2
    if(player2 != "minimax_player"){
      for (int j = i+1; j < population.size(); ++j){
        fixture.push_back(match(population[i], population[j], player1, player2, iter, board));
      }
    } else {
      fixture.push_back(match(population[i], population[j], player1, player2, iter, board));
    }
  }

  return fixture;
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

/*
hile(best_ind.first < benchmark){

    // Calculo el mejor fitness de la poblacion y los ordeno
    //Esto deberia ordenar la poblacion  de mejor a peor fitness
    // y retorar el mejor score

    best_ind.first = rank_population(pop);

    best_ind.second = pop[0];
*/

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




//En player1 o player2 pasar: parametric_player, minimax_player, etc...
pair<matchResults,matchResults> match(vector<int> weights1, vector<int> weights2, string player1, string player2, int iter, matchBoard& board){

  /* Seteamos parametros */
  string cmd = "python c_linea.py --blue_player ./" + player1;
  /* pasamos weights 1 */
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights1.size()); /* cantidad de parametros */

  for (int i = 0; i < weights1.size(); ++i){
    cmd += " " + to_string(weights1[i]);
  }

  /* pasamos weights 2*/
  cmd+= " --red_player ./" + player2;
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights2.size()); /* cantidad de parametros */

  for (int i = 0; i < weights2.size(); ++i){
    cmd += " " + to_string(weights2[i]);
  }

  cmd += " --iterations " + to_string(iter);
  if(w1_first) {
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

  float score_w1 = won/mean_w_time - lost/mean_l_time;

  red_results >> red.won;
  red_results >> red.lost;
  red_results >> red.tied;
  red_results >> red.median_w_time;
  red_results >> red.median_l_time;

  return make_pair(blue,red);
}




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
