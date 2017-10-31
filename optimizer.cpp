#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

int score(/*vector<int> weights,*/ int iter){

	/* Seteamos parametros */
	string cmd = "python c_linea.py --blue_player ./random_player --red_player ./parametric_player --first azul";
	//for (int i = 0; i < weights.size(); ++i){
	// 	cmd += " " + to_string(weights[i]);
	//}
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
	return won;

}

int main(){

	int won = score(100);
	return 0;

}