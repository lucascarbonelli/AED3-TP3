#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


struct Solution{
  vector<int> weights;
  int score;
};


float fitness(vector<int> weights, int iter, int rows , int columns, int c, int p){

	/* Seteamos parametros */
	string cmd = "python c_linea.py --blue_player ./random_player --red_player ./parametric_player";
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights.size()); /* cantidad de parametros */

  for (int i = 0; i < weights.size(); ++i){
	 	cmd += " " + to_string(weights[i]);
	}

  cmd += " --red_player ./parametric_player"
	cmd += " --iterations " + to_string(iter);
	cmd += " --first azul --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";

	/* Corremos el juego que dejara los resultados en res.txt*/
	system(cmd.c_str());
	ifstream res("res.txt");
	int won,lost,tied, mean_l_time, mean_w_time;
	res >> won;
	res >> lost;
	res >> tied;
  res >> mean_l_time;
  res >> mean_w_time;
	cout << "Ganados: "<< won << " | " << "Perdidos: " << lost << " | " << "Empatados: " << tied <<endl;
  //cout << "Ratio:" << won/(iter)<< endl;;

  float score = won*10/mean_w_time - lost/mean_l_time;

}

void backtrack(vector<int>& weights, vector<int>& range, int n, Solution& S,int& iter){

  /* caso base */
  if(n == 0){
    //int score = fitness(weights,50);
    int score =0;
    if(score > S.score ){
      //cout << "Found better score: " << score << endl;
      S.weights = weights;
      S.score = score;
    }
    /* para la ansiedad */
    cout << "Iter: " << iter << endl;
    iter++;
    return;
  }

  for (size_t i = 0; i < range.size(); i++) {
    weights.push_back(range[i]);
    backtrack(weights,range,n-1,S,iter);
    weights.pop_back();
  }
}

vector<int> gridSearch(int params, vector<int> range){
  int iter = 0;
  vector<int> weights;
  Solution S;
  S.weights = weights;
  S.score = -1;

  backtrack(weights,range,params,S,iter);
  return S.weights;

}



/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/

int compareWeights(vector<int> weights1,vector<int> weights2, int iter, int rows , int columns, int c, int p){

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
	cmd += " --first azul --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";

  system(cmd.c_str());
  ifstream res("res.txt");
  int won,lost,tied, mean_l_time, mean_w_time;
  int total_won, avg_mean;

  res >> won;
  res >> lost;
  res >> tied;
  res >> mean_l_time;
  res >> mean_w_time;

  float score = won*10/mean_w_time - lost/mean_l_time;


  /* ahora empieza el jugador con los pesos weights2 */

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
	cmd += " --first rojo --columns "+to_string(columns)+" --rows "+to_string(rows)+" --p "+to_string(p)+" --c "+to_string(c)+" ";


  res >> won;
  res >> lost;
  res >> tied;
  res >> mean_l_time;
  res >> mean_w_time;

  bool better_than_random = (fitness(weights1,100)>=fitness(weights2,100));


  float score += won*15/mean_w_time - lost/mean_l_time;




	/* Corremos el juego que dejara los resultados en res.txt*/

	//cout << "Ganados: "<< won << " | " << "Perdidos: " << lost << " | " << "Empatados: " << tied <<endl;

  /* veamos quien es el mejor */
  //bool better_than_random = (fitness(weights1,100)>=fitness(weights2,100));
  if(won + tied > lost  /*&&better_than_random*/) return 1;
  return 0;

}




vector<int> generateRandomWeights(int params,vector<int> range){

  vector<int> weights;
  for (size_t i = 0; i < params; i++) {
    int e = range[rand() % range.size()];
    weights.push_back(e);
  }
  weights[4] = 100;
  weights[params-1] = 1000000;
  return weights;
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

  vector<int> weights(c+columns+1,0);
  /*
  for (int i = 0; i < c; i++) {
    weights[i] = i;
  }
  for (int i = c ; i < c + columns; i++) {
    weights[i] = -5*i;
  }*/
  weights = {0, 4, -1, 1, 3, -4, 1, 2, -1, -3, 1, 5};
  weights = {-1, 1, 4, 15, 100, -4, 3, -4, 3, -2, 2, 1000000};
  vector<int> range{-15,-10,-5,-4,-3,-2,-1,0,1,2,3,4,5,10,15};

  /* fijamos algunas */
  //weights[c-1] = 1000000;
  weights[c+columns] = 1000000;

  //weights = randomSearch(c+columns+1,range,150,weights);

  cout << "Weights: " << endl;
  for (size_t i = 0; i < weights.size(); i++) {
    cout << weights[i] << " ";
  }
  cout << endl;

  int won = fitness(weights,1000,rows,columns,c,p);

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
