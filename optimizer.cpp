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


vector<int> generateRandomWeights(int params,vector<int> range){

  vector<int> weights;
  for (size_t i = 0; i < params; i++) {
    int e = range[rand() % range.size()];
    weights.push_back(e);
  }
  return weights;
}



void backtrack(vector<int>& weights, vector<int>& range, int n, Solution& S,int& iter,int rows , int columns, int c, int p){

  /* caso base */
  if(n == 0){
    pair<float,float> score = match(weights,S.weights,5,rows,columns,c,p,true);
    if(score.first > score.second ){
      cout << "Found better score: " << score.first << " " << score.second << endl;
      S.weights = weights;
    }
    cout << endl;

    /* para la ansiedad */
    cout << "Iter: " << iter << endl;
    iter++;
    return;
  }

  for (size_t i = 0; i < range.size(); i++) {
    weights.push_back(range[i]);
    backtrack(weights,range,n-1,S,iter,rows,columns,c,p);
    weights.pop_back();
  }
}

vector<int> gridSearch(int params, vector<int> range,vector<int> init_Weights,int rows , int columns, int c, int p){
  int iter = 0;
  int n = params;
  vector<int> weights;
  Solution S;
  S.weights = generateRandomWeights(params,range);

  backtrack(weights,range,n+1,S,iter,rows,columns,c,p);
  return S.weights;

}



/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/




//vector<int> HeuristicGridSeach(int range)





















int compareWeights(vector<int> weights1,vector<int> weights2, int iter, int rows , int columns, int c, int p){

  pair<float,float> scores1 = match(weights1, weights2, iter, rows , columns, c, p,true);
  pair<float,float> scores2 = match(weights1, weights2, iter, rows , columns, c, p,false);
  /* veamos quien es el mejor */
  bool better_than_random = (fitness(weights1,iter,rows,columns,c,p)>=fitness(weights2,iter,rows,columns,c,p));

  if(((scores1.first + scores2.first) > (scores1.second + scores2.second)) && better_than_random ) return 1;
  return 0;
}






vector<int> randomSearch(int params, vector<int> range, int iterations, vector<int>& init_Weights, int rows , int columns, int c, int p ){

  vector<int> weights(params,0);
  vector<int> best_solution = init_Weights;

  for (size_t i = 0; i < iterations; i++) {
    cout << "Iteration: " << i << endl;
    /* generamos una solucion al azar */
    vector<int> sol = generateRandomWeights(params,range);
    /* comaparamos con la mejor hasta el momento */
    if(compareWeights(sol,best_solution,50, rows , columns, c, p) == 1){
        cout << "Found better solution" << endl;
        cout << "Weights: " << endl;
        for (size_t j = 0; j < sol.size(); j++) {
          cout << sol[j] << " ";
        }
        best_solution = sol;
    }

  }
  return best_solution;
}


//10 1 -4 10 100 1 -15 2 0 -1 1 1000000
//-1 1 4 15 100 -4 3 -4 3 -2 2 1000000
//-1 1 4 15 100 -4 3 -4 3 -2 2 1000000













int main(){

  int c = 4;
  int columns = 7;
  int rows = 6;
  int p = 21;

  //vector<int> weights(c+columns+1,0);
  /*
  for (int i = 0; i < c; i++) {
    weights[i] = i;
  }
  for (int i = c ; i < c + columns; i++) {
    weights[i] = -5*i;
  }*/
  //weights = {0, 4, -1, 1, 3, -4, 1, 2, -1, -3, 1, 5};
  //weights = {-1, 1, 4, 15, 100, -4, 3, -4, 3, -2, 2, 1000000};
  //vector<int> range{-5,-2,0,2,5};

  ///* fijamos algunas */
  ////weights[c-1] = 1000000;
  //weights[c+columns] = 1000000;

  vector<int> weights = {4, 15, 1000000, -10, 0, 1, 2, 3, 2, 1, 0,5,10,-6,-11};
  vector<int> range{-100,-5,-2,0,2,5,100};

  //weights = gridSearch(c,range,weights,rows,columns,c,p);

  cout << "Weights: " << endl;
  for (size_t i = 0; i < weights.size(); i++) {
    cout << weights[i] << " ";
  }
  cout << endl;

  float won = fitness(weights,1000,rows,columns,c,p);

	return 0;

}

















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
