#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Board.h"

class Heuristic {
public:
    static int evaluateBoard(const Board& board);
};

#endif