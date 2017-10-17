#include "board.h"
#include <vector>

Board::Board(int cols, int rows, int c, int p){
    _board = std::vector<std::vector<int> >(rows, std::vector<int>(cols, EMPTY));
    _cols = cols;
    _rows = rows;
    _c = c;
    _p = p;
}


Board::Board(const Board& b){
    _board = b._board;
    _cols = b._cols;
    _rows = b._rows;
    _c = b._c;
    _p = b._p;
}


bool Board::isColFull(int col) const{
    return _board[_rows-1][col] != EMPTY;
}

bool Board::isBoardFull() const{
    bool areColsFull = true;

    for(int i = 0; i < _cols; i++){
        if(!isColFull(i)){
            areColsFull = false;
        }
    }

    return areColsFull;
}


bool Board::didIWin() const{
    return isWinner(PLAYER);
}


bool Board::didILost() const{
    return isWinner(OPPONENT);
}


bool Board::isWinner(int player) const{
    bool winner = false;

    // Chequear lineas horizontales
    for(int i = 0; i < _rows; i++){
        for(int j = 0; j < _cols - _c + 1; j++){
            bool winning = true;
            for(int k = 0; k < _c; k++){
                if(_board[i][j + k] != player){ winning = false; }
            }
            if(winning){ winner = true; }
        }
    }

    // Chequear lineas verticales
    for(int i = 0; i < _rows - _c + 1; i++){
        for(int j = 0; j < _cols; j++){
            bool winning = true;
            for(int k = 0; k < _c; k++){
                if(_board[i + k][j] != player){ winning = false; }
            }
            if(winning){ winner = true; }
        }
    }

    // Chequear lineas diagonales (/)
    for(int i = 0; i < _rows - _c + 1; i++){
        for(int j = 0; j < _cols - _c + 1; j++){
            bool winning = true;
            for(int k = 0; k < _c; k++){
                if(_board[i + k][j + k] != player){ winning = false; }
            }
            if(winning){ winner = true; }
        }
    }

    // Chequear lineas diagonales (\)
    for(int i = 0; i < _rows - _c + 1; i++){
        for(int j = 0; j < _cols - _c + 1; j++){
            bool winning = true;
            for(int k = 0; k < _c; k++){
                if(_board[i + k][_cols - (j + k) - 1] != player){ winning = false; }
            }
            if(winning){ winner = true; }
        }
    }

    return winner;
}


int Board::getPieces() const{
    return _p;
}


int Board::getColumns() const{
    return _cols;
}


void Board::addPlayer(int col){
    int row = 0;

    while(_board[row][col] != EMPTY){
        row++;
    }

    _board[row][col] = PLAYER;
    _p--; // Dismiuye la cantidad de fichas
}


void Board::addOpponent(int col){
    int row = 0;
    
        while(_board[row][col] != EMPTY){
            row++;
        }
    
        _board[row][col] = OPPONENT;
        _p--; // Dismiuye la cantidad de fichas
}