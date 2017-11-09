
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;


struct Solution{
  vector<int> weights;
};


//struct Context{
//  int n;
//  int m;
//  int c;
//  int p;
//  vector<int> range;
//
//  vector<int> weights;
//};



random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()





float fitness(vector<int> weights, int iter, int rows , int columns, int c, int p){

	/* Seteamos parametros */
	string cmd = "python c_linea.py --blue_player ./parametric_player ";
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights.size()); /* cantidad de parametros */

  for (int i = 0; i < weights.size(); ++i){
	 	cmd += " " + to_string(weights[i]);
	}

  cmd += " --red_player ./random_player";
	cmd += " --iterations " + to_string(iter);
	cmd += " --first azul --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";

	/* Corremos el juego que dejara los resultados en res.txt*/
	system(cmd.c_str());
	ifstream res("azul.txt");
	int won,lost,tied, median_l_time, median_w_time;
	res >> won;
	res >> lost;
	res >> tied;
  res >> median_w_time;
  res >> median_l_time;
	//cout << "Ganados: "<< won << " | " << "Perdidos: " << lost << " | " << "Empatados: " << tied <<endl;
  //cout << "median_w_time: " << median_w_time << " | " << "median_l_time: " << median_l_time << endl;

  float score = won/median_w_time - lost/median_l_time;
  cout << "Score: " << score << endl;
  cout << "Won " << won << " times."<< endl;
  cout << "Lost " << lost << " times."<< endl;
  cout << "Tied " << tied << " times."<< endl;
  return score;

}

pair<float,float> match(vector<int> weights1,vector<int> weights2, int iter,int rows,int columns,int c, int p, int w1_first){

  /* Seteamos parametros */
  string cmd = "python c_linea.py --blue_player ./parametric_player";
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
  if(w1_first) {
    cmd += " --first azul --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";
  } else {
    cmd += " --first rojo --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";
  }


  system(cmd.c_str());

  ifstream red_results("rojo.txt");
  ifstream blue_results("azul.txt");

  int won,lost,tied, median_l_time, median_w_time;
  int total_won, avg_mean;
  blue_results >> won;
  blue_results >> lost;
  blue_results >> tied;
  blue_results >> median_w_time;
  blue_results >> median_l_time;

  float score_w1 = won - lost;

  red_results >> won;
  red_results >> lost;
  red_results >> tied;
  red_results >> median_w_time;
  red_results >> median_l_time;

  float score_w2 = won - lost;

  return make_pair(score_w1,score_w2);


}





vector<int> generateRandomWeights(int M, int N, int c , int P, vector<int> range){

  std::uniform_int_distribution<> dis(0, range.size() - 1);

  vector<int> init_Weights(3*c + M -5,0);
  for (size_t i = 0; i < c-2; i++) {
    init_Weights[i] = range[dis(gen)];
  }
  init_Weights[c - 2] = range[dis(gen)];;

  // pesos de candida de fichas en columnas
  for (size_t i = 0; i <= M/2 ; i++) {
    init_Weights[c-1 + i] = i;
  }
  for (size_t i = 0; i <  M/2; i++) {
    init_Weights[i + c-1 + M/2] = M/2 - i;
  }


  for (size_t i = c-1 + M ; i < c-1 + M + (c-2); i++) {
    init_Weights[i] = range[dis(gen)];;
  }
  for (size_t i =  c-1 + M + (c-2); i < c-1 + M + (c-2) + (c-2) ; i++) {
    init_Weights[i] = range[dis(gen)];;
  }
  return init_Weights;
}













int compareWeights(vector<int> weights1,vector<int> weights2, int iter, int rows , int columns, int c, int p){

  /* empieza weights1 */
  pair<float,float> scores1 = match(weights1, weights2, iter, rows , columns, c, p,true);
  /* empieza weights2 */
  pair<float,float> scores2 = match(weights1, weights2, iter, rows , columns, c, p,false);
  /* veamos quien es el mejor */
  //cout << "Scores: " << (scores1.first + scores2.first*1.5)<< " | " << (scores1.second + scores2.second*1.5) << endl;
  if( (scores1.first + scores2.first*1.5) > (scores1.second*1.5 + scores2.second) ) {return 1;}
  return 0;
}



void backtrack(vector<int> params_to_optimize, vector<int>& weights, vector<int>& S, vector<int>& range, int n,int& iter,int rows , int columns, int c, int p){

  /* caso base */
  if(n == 0){
    int res = compareWeights(weights,weights,1,rows,columns,c,p);
    if(res==1){
      weights = weights;
    }

    //cout << "Weights: " << endl;
    //for (size_t i = 0; i < weights.size(); i++) {
    //  cout << weights[i] << " ";
    //}
    //cout << endl;

    iter++;
    return;
  }

  for (size_t i = 0; i < range.size(); i++) {
    unsigned int param = params_to_optimize[n-1];
    //cout << "param: " << param <<endl;
    weights[param] = range[i];
    backtrack(params_to_optimize,weights,range,S,n-1,iter,rows,columns,c,p);
  }
}

vector<int> gridSearchHelper(vector<int> params_to_optimize, vector<int> range,vector<int> init_Weights,int rows , int columns, int c, int p){
  int iter = 0;
  int n = params_to_optimize.size();
  vector<int> weights = init_Weights;
  vector<int> S = init_Weights;


  backtrack(params_to_optimize,weights,S,range,n,iter,rows,columns,c,p);
  return S;

}


void exploreNeighbourhood(vector<int>& init_weights, int M, int N, int c , int P, vector<int> range){



  /* optimizamos parametros del primer feature */
  vector<int> params_to_optimize;
  for (int i = 0; i < c-2; i++) params_to_optimize.push_back(i);
  vector<int> weights = gridSearchHelper(params_to_optimize,range,init_weights,N,M,c,P);


  /* optimizamos parametros del segundo feature */
  params_to_optimize.clear();
  params_to_optimize.push_back(c-2);
  weights = gridSearchHelper(params_to_optimize,range,weights,N,M,c,P);


  /* optimizamos parametros del cuarto feature */
  params_to_optimize.clear();
  for (int i = c-1 + M ; i < c-1 + M + (c-2) ; i++) params_to_optimize.push_back(i);
  weights = gridSearchHelper(params_to_optimize,range,weights,N,M,c,P);


  /* optimizamos parametros del quinto feature */
  params_to_optimize.clear();
  for (int i = c-1 + M + (c-2) ; i < c-1 + M + (c-2) +(c-2); i++) params_to_optimize.push_back(i);
  weights = gridSearchHelper(params_to_optimize,range,weights,N,M,c,P);

  //cout << "Resultados contra random" << endl;
  //float won = fitness(weights,1000,N,M,c,P);

}



vector<int> GRASP(int n, int m, int c, int p, vector<int> range, int iterations){

  vector<int> best_solution;


  for (size_t i = 0; i < iterations; i++) {
    vector<int> weights = generateRandomWeights(m,n,c,p,range);
    cout << "Weights generados: " << endl;
    for (size_t i = 0; i < weights.size(); i++) {
      cout << weights[i] << " ";
    }
    cout << endl;
    cout << "[*]Optimizando..." << endl;
    exploreNeighbourhood(weights,m,n,c,p,range);
    cout << "Resultados contra random" << endl;
    float won = fitness(weights,1000,n,m,c,p);

    if(i==0) best_solution = weights;
    else{
      if(compareWeights(weights,best_solution,1,n,m,c,p)) best_solution = weights;
    }

  }


  return best_solution;

}





int main(int argc, char const *argv[]) {

  int columns = 7;
  int rows = 6;
  int c = 4;
  int p = 21;

  vector<int> range = {-10,-5,0,5,10};

  //for (size_t i = 0; i < bounds; i+step) {
  //  range.push_back(i);
  //  range.push_back(-i);
  //}

  vector<int> weights = GRASP(rows,columns,c,p,range,10);
  cout << "Weights finales: " << endl;
  for (size_t i = 0; i < weights.size(); i++) {
    cout << weights[i] << " ";
  }
  cout << "Resultados contra random" << endl;
  float won = fitness(weights,1000,rows,columns,c,p);


  return 0;
}




/*
Algunos resultados
(*) 0 0 5 0 1 2 3 2 1 0 10 10 -5 10 para 4 en linea

*/

















































































//
