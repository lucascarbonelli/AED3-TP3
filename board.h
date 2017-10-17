#ifndef BOARD_H
#define BOARD_H

#include <vector>

#define EMPTY 0
#define PLAYER 1
#define OPPONENT 2

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

private:
    std::vector<std::vector<int> > _board;
    int _cols;
    int _rows;
    int _c; // cantidad de fichas en linea para ganar
    int _p; // cantidad de fichas

    bool isWinner(int player) const;
};

#endif