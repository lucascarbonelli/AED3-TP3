#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


struct Solution{
  vector<int> weights;
};


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

  float score_w1 = won/median_w_time - lost/median_l_time;

  red_results >> won;
  red_results >> lost;
  red_results >> tied;
  red_results >> median_w_time;
  red_results >> median_l_time;

  float score_w2 = won/median_w_time - lost/median_l_time;

  return make_pair(score_w1,score_w2);


}






int compareWeights(vector<int> weights1,vector<int> weights2, int iter, int rows , int columns, int c, int p){

  pair<float,float> scores1 = match(weights1, weights2, iter, rows , columns, c, p,true);
  pair<float,float> scores2 = match(weights1, weights2, iter, rows , columns, c, p,false);
  /* veamos quien es el mejor */
  //bool better_than_random = (fitness(weights1,10,rows,columns,c,p)>=fitness(weights2,10,rows,columns,c,p));

  if(((scores1.first + scores2.first) > (scores1.second + scores2.second)) ) return 1;
  return 0;
}



void backtrack(vector<int> params_to_optimize, vector<int>& weights, vector<int>& range, int n, Solution& S,int& iter,int rows , int columns, int c, int p){

  /* caso base */
  if(n == 0){
    int res = compareWeights(weights,S.weights,1,rows,columns,c,p);
    if(res==1){
      S.weights = weights;
    }
    /* para la ansiedad */
    cout << "Iter: " << iter << endl;
    iter++;
    return;
  }

  for (size_t i = 0; i < range.size(); i++) {
    unsigned int param = params_to_optimize[n];
    weights[param] = range[i];
    backtrack(params_to_optimize,weights,range,n-1,S,iter,rows,columns,c,p);
  }
}

vector<int> gridSearchHelper(vector<int> params_to_optimize, vector<int> range,vector<int> init_Weights,int rows , int columns, int c, int p){
  int iter = 0;
  int n = params_to_optimize.size();
  vector<int> weights(init_Weights.size());
  Solution S;
  S.weights = init_Weights;

  backtrack(params_to_optimize,weights,range,n,S,iter,rows,columns,c,p);
  return S.weights;

}


vector<int> gridSearch(int M, int N, int c , int P, vector<int> range){

  vector<int> init_Weights(3*c + M -5 );
  for (size_t i = 0; i < c-2; i++) {
    init_Weights[i] = 5*(i+1);
  }
  init_Weights[c - 2] = -10;

  // pesos de candida de fichas en columnas
  for (size_t i = 0; i <= M/2 ; i++) {
    init_Weights[c-1 + i] = i;
  }
  for (size_t i = 0; i <  M/2; i++) {
    init_Weights[i + c-1 + M/2] = M/2 - i;
  }


  for (size_t i = c-1 + M ; i < c-1 + M + (c-2); i++) {
    init_Weights[i] = 5 * (i+2 - c - M );
  }
  for (size_t i =  c-1 + M + (c-2); i < c-1 + M + (c-2) + (c-2) ; i++) {
    init_Weights[i] = -5 * (i+2 - c - M -(c-2)) - 1;
  }

  cout << "Weights: " << endl;
  for (size_t i = 0; i < init_Weights.size(); i++) {
    cout << init_Weights[i] << " ";
  }


  /* optimizamos parametros del primer feature */
  vector<int> params_to_optimize;
  for (size_t i = 0; i < c-2; i++) params_to_optimize.push_back(i);
  vector<int> weights = gridSearchHelper(params_to_optimize,range,init_Weights,N,M,c,P);


  /* optimizamos parametros del segundo feature */
  params_to_optimize.clear();
  params_to_optimize.push_back(c-2);
  weights = gridSearchHelper(params_to_optimize,range,weights,N,M,c,P);


  /* optimizamos parametros del cuarto feature */
  params_to_optimize.clear();
  for (size_t i = c-1 + M ; i < c-1 + M + (c-2) ; i++) params_to_optimize.push_back(i);
  weights = gridSearchHelper(params_to_optimize,range,weights,N,M,c,P);


  /* optimizamos parametros del quinto feature */
  params_to_optimize.clear();
  for (size_t i = c-1 + M + (c-2) ; i < c-1 + M + (c-2) +(c-2); i++) params_to_optimize.push_back(i);
  weights = gridSearchHelper(params_to_optimize,range,weights,N,M,c,P);

  return weights;
}



/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/



int main(int argc, const char* argv[]){

  int columns = 7;
  int rows = 6;
  int c = 4;
  int p = 21;
  int bounds = 17;
  int step = 1;

  //if(argc >= 1){
  //  columns = atoi(argv[1]);
  //  rows = atoi(argv[2]);
  //  c = atoi(argv[3]);
  //  p = atoi(argv[4]);
  //}
  //if(argc >= 6) bounds = atoi(argv[6]);
  //if(argc >= 7) step = atoi(argv[7]);




  //vector<int> range;
  //for (int i = 0 ; i < bounds; i++) {
  //  range.push_back(-i);
  //}
  //for (int i = 0 ; i < bounds; i++) {
  //  range.push_back(i);
  //}
  vector<int> weights = {58,56,-36,-40,89,-17,43,97,105,-14,52,81,26,28};

  cout << "Weights: " << endl;
  for (size_t i = 0; i < weights.size(); i++) {
    cout << weights[i] << " ";
  }
  cout << endl;

  cout << "Resultados contra random" << endl;
  float won = fitness(weights,1000,rows,columns,c,p);

	return 0;

}
























//vector<int> randomSearch(int params, vector<int> range, int iterations, vector<int>& init_Weights, int rows , int columns, int c, int p ){
//
//  vector<int> weights(params,0);
//  vector<int> best_solution = init_Weights;
//
//  for (size_t i = 0; i < iterations; i++) {
//    cout << "Iteration: " << i << endl;
//    /* generamos una solucion al azar */
//    vector<int> sol = generateRandomWeights(params,range);
//    /* comaparamos con la mejor hasta el momento */
//    if(compareWeights(sol,best_solution,50, rows , columns, c, p) == 1){
//        cout << "Found better solution" << endl;
//        cout << "Weights: " << endl;
//        for (size_t j = 0; j < sol.size(); j++) {
//          cout << sol[j] << " ";
//        }
//        best_solution = sol;
//    }
//
//  }
//  return best_solution;
//}
//

//10 1 -4 10 100 1 -15 2 0 -1 1 1000000
//-1 1 4 15 100 -4 3 -4 3 -2 2 1000000
//-1 1 4 15 100 -4 3 -4 3 -2 2 1000000































//
//int main(){
//
//  int c = 4;
//  int columns = 7;
//
//  vector<int> weights(c +columns+1,0);
//
//  for (int i = 0; i < c; i++) {
//    weights[i] = i;
//  }
//  for (int i = c ; i < c + columns; i++) {
//    weights[i] = -5*i;
//  }
//
//  int best_i,best_j;
//  int best_score;
//
//  weights[c-1] = 10*7;
//  weights[c+columns -1] = 10*4;
//
//  weights[c+columns] = 20000000;
//
//  vector<int> range{0,10,50,100,1000};
//  weights = gridSearch(c+columns+1,range);
//
//  cout << "Weights: " << endl;
//  for (size_t i = 0; i < weights.size(); i++) {
//    cout << weights[i] << " ";
//  }
//  cout << endl;
//
//  //for (size_t i = 0; i < 10; i++) {
//  //  for (size_t j = 0; j < 10; j++) {
//
//  //    cout << "Iteration: " << 10*i +j << endl;
//
//  //    weights[c-1] = 10*i;
//  //    weights[c+columns -1] = 10*j;
//  //    int won = score(weights,100);
//  //    if(won > best_score){
//  //      best_score = won;
//  //      best_i = i;
//  //      best_j = j;
//  //    }
//
//
//  //  }
//  //}
//
//  //for (size_t i = 0; i < count; i++) {
//  //  /* code */
//  //}
//
//  int won = fitness(weights,1000);
//
//	return 0;
//
//}
//
