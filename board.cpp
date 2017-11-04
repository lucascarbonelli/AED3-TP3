#include "board.h"
#include <vector>
using namespace std;

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



/*********************************************************/

void Board::backtrack(int col){

  if(isColFull(col)){
    _board[_rows - 1][col] = EMPTY;
  } else {
    int row = 0;
    while(_board[row][col] != EMPTY){
        row++;
    }
    _board[row-1][col] = EMPTY;
  }

  _p++; // Aumenta la cantidad de fichas

}


/*********************************************************/
/*********************** FEATURES ************************/
/*********************************************************/



int Board::horizontal(int i, int j, int player){
  /* Chequear lineas diagonales (\) */
  int left = 0,right = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = j + k < _cols;
    if( ok &&  _board[i][j + k] == player) right++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = 0 <= j-k;
    if( ok &&  _board[i][j - k] == player) left++;
    else break;
  }
  return (left + 1 + right);
}


int Board::vertical(int i, int j, int player){
  /* Chequear lineas verticales */
  int up = 0, down = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = i + k < _rows;
    if( ok &&  _board[i+k][j] == player) up++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = 0 <= i-k;
    if( ok &&  _board[i-k][j] == player) down++;
    else break;
  }
  return (up + 1 + down);
}


int Board::diagonalR(int i, int j, int player){
  /* Chequear lineas diagonales(/) */
  int up = 0, down = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = (j + k < _rows) && (i + k < _rows);
    if( ok &&  _board[i + k][j + k] == player) up++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = (j - k > 0) && (i - k > 0);
    if( ok &&  _board[i - k][j - k] == player) down++;
    else break;
  }
  return (up + 1 + down);
}





int Board::diagonalL(int i, int j, int player){
  /* Chequear lineas diagonales (\) */
  int up = 0,down = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = (i+k < _rows) && (0 <= j-k);
    if( ok &&  _board[i + k][j - k] == player) up++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = (0 <= i-k) && ( j+k < _cols);
    if( ok && _board[i-k][j+k] == player) down++;
    else break;
  }
  return up + 1 + down;
}


/***********************************************************/

vector<int> Board::lineCounts(int player){
  // contamos lineas de longitud 2 hasta longitud c


  vector<int> count(_c - 1,0);

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      if(_board[i][j] == player){
        if(horizontal(i,j,player) >= 2) count[horizontal(i,j,player)-2]++;
        if(vertical(i,j,player) >=2) count[vertical(i,j,player)-2]++;
        if(diagonalL(i,j,player) >=2) count[diagonalL(i,j,player)-2]++;
        if(diagonalR(i,j,player) > 2) count[diagonalR(i,j,player)-2]++;
      }
    }
  }

  for (int i = 0; i < _c - 1; i++) {
    /* eliminamos repetidos */
    count[i] = count[i]/(i+2);
  }
  return count;
}

/***********************************************************/


vector<int> Board::possibleMoves(){
  vector<int> rows(_cols,-1);
  for (size_t i = 0; i < _cols; i++) {
    if(!isColFull(i)){
      int row = 0;
      while(_board[row][i] != EMPTY){
          row++;
      }
      rows[i] = row;
    }
  }
  return rows;
}


vector<int> Board::player_prof(unsigned int player){
  // lineas de mayor longitud que puede generar el jugador

  vector<int> rows = possibleMoves();
  vector<int> res(_cols,0);
  for (size_t i = 0; i < _cols; i++) {
    if(rows[i] != -1){
      int m1 = max(horizontal(rows[i],i,player),vertical(rows[i],i,player));
      int m2 = max(diagonalR(rows[i],i,player),diagonalL(rows[i],i,player));
      res[i] = max(m1,m2);
    }
  }
  return res;
}


/****************************************************************************/

vector<int> Board::columnsCount(unsigned int player){
  vector<int> count(_cols);
  for (size_t j = 0; j < _cols; j++) {
    for (size_t i = 0; i < _rows; i++) {
      if(_board[i][j] == player) count[j]++;
      if(_board[i][j] == EMPTY) break;
    }
  }
  return count;
}




/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


pair<int,int> Board::openHorizontal(int i, int j, int player){
  /* Chequear lineas diagonales (\) */
  int left = 0,right = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = j + k < _cols;
    if( ok &&  _board[i][j + k] == player) right++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = 0 <= j-k;
    if( ok &&  _board[i][j - k] == player) left++;
    else break;
  }
  return make_pair(left,right);
}


pair<int,int> Board::openVertical(int i, int j, int player){
  /* Chequear lineas verticales */
  int up = 0, down = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = i + k < _rows;
    if( ok &&  _board[i+k][j] == player) up++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = 0 <= i-k;
    if( ok &&  _board[i-k][j] == player) down++;
    else break;
  }
  return make_pair(up,down);
}


pair<int,int> Board::openDiagonalR(int i, int j, int player){
  /* Chequear lineas diagonales(/) */
  int up = 0, down = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = (j + k < _rows) && (i + k < _rows);
    if( ok &&  _board[i + k][j + k] == player) up++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = (j - k > 0) && (i - k > 0);
    if( ok &&  _board[i - k][j - k] == player) down++;
    else break;
  }
  return make_pair(up,down);
}





pair<int,int> Board::openDiagonalL(int i, int j, int player){
  /* Chequear lineas diagonales (\) */
  int up = 0,down = 0;

  for (int k = 1; k <= _c ; k++) {
    bool ok = (i+k < _rows) && (0 <= j-k);
    if( ok &&  _board[i + k][j - k] == player) up++;
    else break;
  }
  for (int k = 1; k <= _c ; k++) {
    bool ok = (0 <= i-k) && ( j+k < _cols);
    if( ok && _board[i-k][j+k] == player) down++;
    else break;
  }
  return make_pair(up,down);
}



vector<int> Board::countOpen(unsigned int player){

  vector<int> count(_c-2,0);

  for (size_t i = 0; i < _rows; i++) {
    for (size_t j = 0; j < _cols; j++) {
      if(_board[i][j] == EMPTY){
        pair<int,int> oH = openHorizontal(i,j,player);
        if(oH.first > 2) count[oH.first-3]++;
        if(oH.second > 2) count[oH.second-3]++;
        pair<int,int> oV = openVertical(i,j,player);
        if(oV.first > 2) count[oV.first-3]++;
        if(oV.second > 2) count[oV.second-3]++;
        pair<int,int> oDL = openDiagonalL(i,j,player);
        if(oDL.first > 2) count[oDL.first-3]++;
        if(oDL.second > 2) count[oDL.second-3]++;
        pair<int,int> oDR = openDiagonalR(i,j,player);
        if(oDR.first > 2) count[oDR.first-3]++;
        if(oDR.second > 2) count[oDR.second-3]++;
      }
    }
  }

  return count;

}









































//
