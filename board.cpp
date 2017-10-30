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


int Board::horizonal(int i, int j, int player){
  /* Chequear lineas horizontales */
  int left = 0,right = 0;

  for (int k = j + 1; k < min(j+_c,_cols); k++) {
    if(_board[i][k] == player) right++;
    else break;
  }
  for (int k = j - 1; k >= max(j-_c,0); k--) {
    if(_board[i][k] == player) left++;
    else break;
  }

  return left + 1 + right;
}

int Board::vertical(int i, int j, int player){

  int up = 0,down = 0;

  for (int k = i + 1; k < min(i+_c,_rows); k++) {
    if(_board[k][j] == player) up++;
    else break;
  }
  for (int k = i - 1; k >= max(i-_c,0); k--) {
    if(_board[k][j] == player) down++;
    else break;
  }

  return up + 1 + down;

}


int Board::diagonalR(int i, int j, int player){
  /* Chequear lineas diagonales (/) */
  int up = 0,down = 0;

  for (int k = 1; k < min(i+_c, min(_rows, _cols)) - i ; k++) {
    if(_board[i + k][j + k] == player) up++;
    else break;
  }
  for (int k = 1; k < i - max(i-_c, max(j-_c,0)); k++) {
    if(_board[i - k][j - k] == player) down++;
    else break;
  }

  return up + 1 + down;

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

  vector<int> count(_c,0);

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      if(_board[i][j] == player){
        count[horizonal(i,j,player)]++;
        count[vertical(i,j,player)]++;
        count[diagonalL(i,j,player)]++;
        count[diagonalR(i,j,player)]++;
      }
    }
  }

  for (int i = 0; i < count.size(); i++) {
    /* eliminamos repetidos */
    count[i] = count[i] / (i+1);
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
      int m1 = max(horizonal(rows[i],i,player),vertical(rows[i],i,player));
      int m2 = max(diagonalR(rows[i],i,player),diagonalL(rows[i],i,player));
      res[i] = max(m1,m2);
    }
  }
  return res;
}








unsigned int Board::linesOfSize(int n, int player){

/* Calcula para un tablero dado la cantidad de lineas de
 * longitud *n* que formo el jugador *player*.
 *
 * Debe cumplirse: 0 < n < _c
 */


  unsigned int count = 0;
  // Chequear lineas horizontales
  for(int i = 0; i < _rows; i++){
      for(int j = 0; j < _cols - n + 1; j++){
          bool same_player = true;
          for(int k = 0; k < n; k++){
              if(_board[i][j + k] != player){ same_player = false; }
          }
          /* Nos fijamos que la linea no forme parte de otra linea de mayor tamaño. */
          //if(_board[i][j + n] == player) Learning to play Othello with n-tuple systems. Asame_player = false;
          if(same_player) count++;
      }
  }

  // Chequear lineas verticales
  for(int i = 0; i < _rows - n + 1; i++){
      for(int j = 0; j < _cols; j++){
          bool same_player = true;
          for(int k = 0; k < n; k++){
              if(_board[i + k][j] != player){ same_player = false; }
          }
          /* Nos fijamos que la linea no forme parte de otra linea de mayor tamaño. */
          //if(_board[i + n][j] == player) same_player = false;
          if(same_player) count++;
      }
  }

  // Chequear lineas diagonales (/)
  for(int i = 0; i < _rows - n + 1; i++){
      for(int j = 0; j < _cols - n + 1; j++){
          bool same_player = true;
          for(int k = 0; k < n; k++){
              if(_board[i + k][j + k] != player){ same_player = false; }
          }
          /* Nos fijamos que la linea no forme parte de otra linea de mayor tamaño. */
          //if(_board[i + n][j + n] == player) same_player = false;
          if(same_player) count++;
      }
  }

  // Chequear lineas diagonales (\)
  for(int i = 0; i < _rows - n + 1; i++){
      for(int j = 0; j < _cols - n + 1; j++){
          bool same_player = true;
          for(int k = 0; k < n; k++){
              if(_board[i + k][_cols - (j + k) - 1] != player){ same_player = false; }
          }
          /* Nos fijamos que la linea no forme parte de otra linea de mayor tamaño. */
          //if(_board[i + n][_cols - (j + n) - 1]  == player) same_player = false;
          if(same_player) count++;
      }
  }
  return count;
}


















































//
