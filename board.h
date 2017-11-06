#ifndef BOARD_H
#define BOARD_H

#include <vector>

#define EMPTY 0
#define PLAYER 1
#define OPPONENT 2
using namespace std;
class Board{
public:
    Board(int cols, int rows, int c, int p);
    Board(const Board& b);
    bool isColFull(int col) const;
    bool isBoardFull() const;
    bool didIWin() const;
    bool didILost() const;
    int getPieces() const;
    int getColumns() const;
    void addPlayer(int col);
    void addOpponent(int col);

    /* Agregadas por nosotros: */
    void backtrack(int col);
    int horizontal(int i, int j, int player);
    int vertical(int i, int j, int player);
    int diagonalR(int i, int j, int player);
    int diagonalL(int i, int j, int player);
    vector<int> lineCounts(int player);
    unsigned int linesOfSize(int n, int player);
    vector<int> possibleMoves();
    vector<int> player_prof(unsigned int player);
    vector<int> columnsCount(int player);
    pair<int,int> openHorizontal(int i, int j, int player);
    pair<int,int> openVertical(int i, int j, int player);
    pair<int,int> openDiagonalR(int i, int j, int player);
    pair<int,int> openDiagonalL(int i, int j, int player);
    vector<int> countOpen(unsigned int player);
    int LosingHazard();

private:
    std::vector<std::vector<int> > _board;
    int _cols;
    int _rows;
    int _c; // cantidad de fichas en linea para ganar
    int _p; // cantidad de fichas

    bool isWinner(int player) const;
};

#endif
