#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <utility>    //pair
#include <algorithm>  //sort
#include <vector>

using namespace std;

typedef vector<int> individual;


//Ḿétodos

void init_rnd_population(vector<individual>& population, unsigned int max);
void init_population(vector<individual>& population);
void selection_helix(int n, int m , int c, int p, bool red_player_first, vector<individual>& population, vector<individual>& new_population, int news, int breeds, string fitness, int quantInd_a_Cross);
void get_better_ones(int n, int m , int c, int p, bool red_player_first, vector<individual>& better_ones, vector<individual>& population, string fitness);
vector<individual> crossover_twopops(vector<individual>& population_a, vector<individual>& population_b, int quantInd_a);
individual breed(individual& individual_a, individual& individual_b, int quantInd_a);
int fitness_himself(int n, int m , int c, int p, bool red_player_first, individual& population);
int fitness_others(int n, int m , int c, int p, bool red_player_first, individual& population);
void mutation(vector<individual>& population, int prob);

//Auxiliares

void save_population(vector<individual> population, int n, int m, int c, int p);
bool pairCompare(const pair<int, unsigned int>& firstElem, const pair<int, unsigned int>& secondElem);