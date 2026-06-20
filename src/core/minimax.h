#ifndef MINIMAX_H
#define MINIMAX_H

#include "Board.h"

class Minimax {
private:
    int alphaBeta(Board& board, int depth, int alpha, int beta, bool isMax, int lastR, int lastC, int aiPlayer);

public:
    // Thay đổi: Nhận thêm thông số level (1-10) và quân cờ mà AI đang nắm giữ
    Move findBestMove(Board& board, int level, int aiPlayer);
};

#endif