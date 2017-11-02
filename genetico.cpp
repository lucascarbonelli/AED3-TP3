#include <stdlib.h>
#include <vector>
typedef vector<int> individual;



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

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < num_features; j++) {
      population[i][j] = rand() % max;
    }
  }
}

//size >= population.size() - cut
void selection_helix(int n, int m , int c, int p, bool red_player_first, vector<individual>& population, int cut, vector<individual>& new_population){

  for (size_t i = cut; i < population.size(); i++){
    new_population[cut-i] = population[i];
  }

  breed_with_better(n, m , c, p, red_player_first, population, cut, new_population);

}

breed_with_better(int n, int m , int c, int p, bool red_player_first, vector<individual>& population, int cut, vector<individual>& new_population){
  //agarra de population hasta cat, toma los np.size-cut mejores, y reproducilos con los mejores de population entero

  vector<individual> better_ones;
  //agarramos los np.size-cut mejores de population
  get_better_ones(n, m , c, p, red_player_first, population, new_population.size() - cut);

  vector<individual> ordered_individuals(population.size()-cut);
  //ordenamos los np.size - cut individuos
  order_individuals(n, m , c, p, red_player_first, population, ordered_individuals);
  //reproducimos a ordered_individuals con better_ones, cero es mitad mitad
  breed(ordered_individuals, better_ones, 0);

  for (size_t i = population.size()-cut; i < new_population.size(); i++){
    new_population[i] = ordered_individuals[i-population.size()-cut];
  }

}











//Auxiliares

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
    if( errno == EEXIST ){
       // alredy exists
    //} else {
       // something else
        //std::cout << "cannot create sessionnamefolder error:" << strerror(errno) << std::endl;
        //throw std::runtime_exception( strerror(errno) );
    //}
    }
  }

  string f_name = "_I"+to_string(population.size())+"_F"+to_string(population[0].size());
  ofstream file(f_name);

  for (size_t i = 0; i < population.size(); i++) {
    for (size_t j = 0; j < population[i].size(); j++) {
      f_name << population[i][j] << " ";
    }
    f_name << endl;
  }



}
