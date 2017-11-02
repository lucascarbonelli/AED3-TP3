#include "genetic.h"


void init_rnd_population(vector<individual>& population, unsigned int max){
  unsigned int size = population.size();
  unsigned int num_features = population[0].size();

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < num_features; j++) {
      population[i][j] = rand() % max;
    }
  }

}

void init_population(vector<individual>& population){

//para hacer un init mas inteligente sobre "los mejores" o "los peores"
}



void helix(matchBoard board, vector<individual>& population, vector<individual>& new_population, paramsGen params){

  for (int i = 0; i < params.news; ++i){
    new_population.push_back(population[population.size()-1-i]);
  }

  vector<individual> better_ones(params.breeds);
  get_better_ones(board, better_ones, population, params.fitness, params.iter);

  vector<individual> old_population;
  for (int i = 0; i < population.size()-params.news; ++i){
    old_population.push_back(population[i]);
  }

  vector<individual> old_better_ones(params.breeds);
  get_better_ones(board, old_better_ones, old_population, params.fitness, params.iter);


  old_better_ones = crossover_twopops(old_better_ones, better_ones, params.quantInd_a_Cross);

  for (int i = 0; i < params.breeds; ++i){
    new_population.push_back(old_better_ones[i]);
  }

  mutation(new_population, params.probMut, params.maxMut);

  //save_population(new_population, n, m, c, p);

}

//copia a better_ones los better_ones.size() mejores según fitness (notar que hay que pasar vector inicializado con tamaño)
void get_better_ones(matchBoard board, vector<individual>& better_ones, vector<individual>& population, string fitness, int iter){

  //first es score y second es indice del individuo en population
  vector<pair<int, unsigned int> > scores(population.size());
  if(fitness == "population"){
  	scores = fitness_population(board, population);
	}

	if(fitness == "others"){
    scores = fitness_others(board, population, iter);
  }

  //lo ordeno con pairCompare
  sort(scores.begin(), scores.end(), pairCompare);

  //meto todo en better_ones
  for (int i = 0; i < better_ones.size(); ++i){
    better_ones[i] = population[scores[population.size()-1-i].second];
  }
}

//este método mezcla los individuos entre dos populations
//puede hacerse otro método que se llame breed y mezcle los individuos de una sola population
vector<individual> crossover_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a){

  vector<individual> res;

  int min_population = population_a.size();
  if(population_b < population_a) min_population = population_b.size();

  for (int i = 0; i < min_population; ++i){
    res.push_back(breed(population_a[i], population_b[i], quantInd_a));
  }

  return res;
}

//toma quantInd_a de individual_a para el res, y el resto de _b
//por ejemplo, quantInd_a = individual_a.size()/2 daría mitad de uno y otro
//CUIDADO: toma ints, así que hacer ceil o floor
individual breed(individual& individual_a, individual& individual_b, int quantInd_a){
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