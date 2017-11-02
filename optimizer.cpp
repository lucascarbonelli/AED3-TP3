#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


struct Solution{
  vector<int> weights;
  int score;
};


int fitness(vector<int> weights, int iter){

	/* Seteamos parametros */
	string cmd = "python c_linea.py --blue_player ./random_player --red_player ./parametric_player";
  cmd += " " + to_string(iter); /* cantidad de iteraciones */
  cmd += " " + to_string(weights.size()); /* cantidad de parametros */

  for (int i = 0; i < weights.size(); ++i){
	 	cmd += " " + to_string(weights[i]);
	}

	cmd += " --iterations " + to_string(iter);
	cmd += " --columns 7 --rows 6 --p 21 --c 4";

	/* Corremos el juego que dejara los resultados en res.txt*/
	system(cmd.c_str());
	ifstream res("res.txt");
	int won,lost,tied;
	res >> won;
	res >> lost;
	res >> tied;
	cout << "Ganados: "<< won << " | " << "Perdidos: " << lost << " | " << "Empatados: " << tied <<endl;
  //cout << "Ratio:" << won/(iter)<< endl;;
	return won;
}

void backtrack(vector<int>& weights, vector<int>& range, int n, Solution& S,int& iter){

  /* caso base */
  if(n == 0){
    int score = fitness(weights,50);
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

int compareWeights(vector<int> weights1,vector<int> weights2, int iter){

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
	cmd += " --first azul --columns 7 --rows 6 --p 21 --c 4";

	/* Corremos el juego que dejara los resultados en res.txt*/
	system(cmd.c_str());
	ifstream res("res.txt");
	int won,lost,tied;
	res >> won;
	res >> lost;
	res >> tied;
	//cout << "Ganados: "<< won << " | " << "Perdidos: " << lost << " | " << "Empatados: " << tied <<endl;

  /* veamos quien es el mejor */
  if(won + tied > lost) return 1;
  return 0;

}




vector<int> generateRandomWeights(int params,vector<int> range){

  vector<int> weights;
  for (size_t i = 0; i < params; i++) {
    int e = range[rand() % range.size()];
    weights.push_back(e);
  }
  return weights;
}



vector<int> randomSearch(int params, vector<int> range, int iterations, vector<int>& init_Weights){

  vector<int> weights(params,0);
  vector<int> best_solution = init_Weights;

  for (size_t i = 0; i < iterations; i++) {
    cout << "Iteration: " << i << endl;
    /* generamos una solucion al azar */
    vector<int> sol = generateRandomWeights(params,range);
    /* comaparamos con la mejor hasta el momento */
    if(compareWeights(sol,best_solution,50) == 1){
        cout << "Found better solution" << endl;
        best_solution = sol;
    }

  }
  return best_solution;
}















int main(){

  int c = 4;
  int columns = 7;
  vector<int> weights(c +columns+1,0);

  for (int i = 0; i < c; i++) {
    weights[i] = i;
  }
  for (int i = c ; i < c + columns; i++) {
    weights[i] = -5*i;
  }

  vector<int> range{-1000, -100, -50, -10, 0, 10, 50, 100, 1000};
  weights = randomSearch(c+columns+1,range,50,weights);

  cout << "Weights: " << endl;
  for (size_t i = 0; i < weights.size(); i++) {
    cout << weights[i] << " ";
  }
  cout << endl;

  int won = fitness(weights,1000);

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
