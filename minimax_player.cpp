#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>

#include "board.h"

// Ganar = 1
// Empatar = 0
// Perder = -1
// minimizar = FALSE
// maximizar = TRUE
int minimax(Board &b, bool miniMax){
    int result;

    if(b.didIWin()) {result = 1;}
    else if(b.didILost()) {result = -1;}
    else if(b.getPieces() == 0 || b.isBoardFull()) {result = 0;} // Empate
    else{
        if(miniMax){
            // Buscamos maximizar
            result = -1;
            for(int i = 0; i < b.getColumns(); i++){
                if(!b.isColFull(i)){
                    Board nextLevelBoard(b);
                    nextLevelBoard.addPlayer(i);
                    int maxInThisBranch = minimax(nextLevelBoard, false);
                    if(maxInThisBranch > result) {result = maxInThisBranch; }
                }
            }
        }else{
            // Buscamos minimizar
            result = 1;
            for(int i = 0; i < b.getColumns(); i++){
                if(!b.isColFull(i)){
                    Board nextLevelBoard(b);
                    nextLevelBoard.addOpponent(i);
                    int maxInThisBranch = minimax(nextLevelBoard, true);
                    if(maxInThisBranch < result) {result = maxInThisBranch; }
                }
            }
        }
    }

    return result;
}

void send(const std::string& msg) {
    std::cout << msg << std::endl;
}

void send(int msg) {
    std::cout << msg << std::endl;
}

int read_int() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return std::stoi(msg);
}

std::string read_str() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return msg;
}

int main() {

    std::string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

    while (true) {
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        Board board(columns, rows, c, 2*p); // 2*p fichas en total

        // Primer movimiento
        go_first = read_str();
        if (go_first == "vos"){
            std::vector<int> win;
            std::vector<int> tie;
            std::vector<int> loss;

            for(int i = 0; i < board.getColumns(); i++){
                Board nextLevelBoard(board);
                nextLevelBoard.addPlayer(i);
                int maxInThisBranch = minimax(nextLevelBoard, false);
                if(maxInThisBranch == 1){ win.push_back(i); }
                else if(maxInThisBranch == 0){ tie.push_back(i); }
                else{ loss.push_back(i); }
            }

            if(!win.empty()){ move = win[0]; }
            else if(!tie.empty()){ move = tie[0]; }
            else{ move = loss[0]; }

            board.addPlayer(move);
            send(move);
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }
            
            board.addOpponent(std::stoi(msg));

            std::vector<int> win;
            std::vector<int> tie;
            std::vector<int> loss;

            for(int i = 0; i < board.getColumns(); i++){
                if(!board.isColFull(i)){
                    Board nextLevelBoard(board);
                    nextLevelBoard.addPlayer(i);
                    int maxInThisBranch = minimax(nextLevelBoard, false);
                    if(maxInThisBranch == 1){ win.push_back(i); }
                    else if(maxInThisBranch == 0){ tie.push_back(i); }
                    else{ loss.push_back(i); }
                }
            }

            if(!win.empty()){ move = win[0]; }
            else if(!tie.empty()){ move = tie[0]; }
            else{ move = loss[0]; }

            board.addPlayer(move);
            
            send(move);
        }
    }

    return 0;
}