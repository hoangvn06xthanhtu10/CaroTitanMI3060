#ifndef BOARD_H
#define BOARD_H

#include <vector>

const int BOARD_SIZE = 15;
const int EMPTY = 0;
const int PLAYER_X = 1;  // Người chơi
const int PLAYER_O = -1; // AI

struct Move {
    int row, col;
    int score;
};

class Board {
private:
    int cells[BOARD_SIZE * BOARD_SIZE];

public:
    Board();
    void reset();
    int getCell(int r, int c) const;
    void makeMove(int r, int c, int player);
    void undoMove(int r, int c);
    bool isValidMove(int r, int c) const;
    bool checkWin(int r, int c, int player) const;
    std::vector<Move> getGeneratorMoves() const; // Chỉ lấy các ô trống quanh ô đã đánh để thu hẹp không gian tìm kiếm
    std::vector<Move> getForcedMoves(int player);
};

#endif