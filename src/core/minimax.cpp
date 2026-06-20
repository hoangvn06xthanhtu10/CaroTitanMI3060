#include "minimax.h"
#include "heuristic.h"
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int INF = std::numeric_limits<int>::max();

int Minimax::alphaBeta(Board& board, int depth, int alpha, int beta, bool isMax, int lastR, int lastC, int aiPlayer) {
    int opponent = (aiPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    
    if (lastR != -1 && board.checkWin(lastR, lastC, isMax ? opponent : aiPlayer)) {
        return isMax ? -1000000 + (5 - depth) : 1000000 - (5 - depth);
    }
    if (depth == 0) {
        int score = Heuristic::evaluateBoard(board);
        return (aiPlayer == PLAYER_O) ? score : -score; 
    }

    std::vector<Move> possibleMoves = board.getGeneratorMoves();
    if (possibleMoves.empty()) return 0;

    for (auto& move : possibleMoves) {
        board.makeMove(move.row, move.col, isMax ? aiPlayer : opponent);
        move.score = Heuristic::evaluateBoard(board);
        board.undoMove(move.row, move.col);
    }

    if (isMax) {
        std::sort(possibleMoves.begin(), possibleMoves.end(), [](const Move& a, const Move& b) {
            return a.score > b.score;
        });
    } else {
        std::sort(possibleMoves.begin(), possibleMoves.end(), [](const Move& a, const Move& b) {
            return a.score < b.score;
        });
    }

    if (possibleMoves.size() > 12) {
        possibleMoves.resize(12);
    }

    if (isMax) {
        int maxEval = -INF;
        for (auto& move : possibleMoves) {
            board.makeMove(move.row, move.col, aiPlayer);
            int eval = alphaBeta(board, depth - 1, alpha, beta, false, move.row, move.col, aiPlayer);
            board.undoMove(move.row, move.col);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = INF;
        for (auto& move : possibleMoves) {
            board.makeMove(move.row, move.col, opponent);
            int eval = alphaBeta(board, depth - 1, alpha, beta, true, move.row, move.col, aiPlayer);
            board.undoMove(move.row, move.col);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

Move Minimax::findBestMove(Board& board, int level, int aiPlayer) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int opponent = (aiPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;

    // --- BƯỚC 1: TẤN CÔNG TUYỆT SÁT - GIỮ NGUYÊN (AI thấy đường thắng là phải ăn ngay) ---
    std::vector<Move> localMoves = board.getGeneratorMoves();
    for (const auto& move : localMoves) {
        board.makeMove(move.row, move.col, aiPlayer);
        if (board.checkWin(move.row, move.col, aiPlayer)) {
            board.undoMove(move.row, move.col);
            std::cout << "[AI Tactical Alert] Instant win executed.\n";
            return move;
        }
        board.undoMove(move.row, move.col);
    }

    // --- BƯỚC 2: PHÒNG NGỰ CÓ XÁC SUẤT BỎ SÓT (CÂN BẰNG ĐỘ KHÓ) ---
    // Thiết lập tỷ lệ phản xạ chặn cờ dựa trên cấp độ
    bool willBlock = true;
    if (level < 7) { 
        int rollDice = std::rand() % 100;
        // Level 1: Chỉ có 20% cơ hội chặn đúng (80% bỏ sót chuỗi 3 mở)
        // Level 3: Có 50% cơ hội chặn đúng
        // Level 6: Có 80% cơ hội chặn đúng
        if (rollDice > (level * 15 + 5)) {
            willBlock = false; 
        }
    }

    // Nếu xúc xắc ra lệnh phải chặn HOẶC đang ở chế độ Hard (Level 7-10)
    if (willBlock) {
        std::vector<Move> forcedMoves = board.getForcedMoves(aiPlayer);
        if (!forcedMoves.empty()) {
            std::cout << "[AI Tactical Alert] Critical defense activated (Level " << level << ").\n";
            return forcedMoves[0]; 
        }
    } else {
        std::cout << "[AI Casual Note] AI Level " << level << " missed a tactical threat!\n";
    }

    // --- BƯỚC 3: DUYỆT CÂY MINIMAX CHO CÁC THẾ TRẬN BÌNH THƯỜNG ---
    Move bestMove = {-1, -1, -INF};
    int bestHeuristicScore = -INF;
    std::vector<Move> possibleMoves = board.getGeneratorMoves();
    int depth = (level + 1) / 2; 
    int noiseRange = (10 - level) * 30; 

    for (auto& move : possibleMoves) {
        board.makeMove(move.row, move.col, aiPlayer);
        move.score = Heuristic::evaluateBoard(board);
        board.undoMove(move.row, move.col);
    }
    
    std::sort(possibleMoves.begin(), possibleMoves.end(), [](const Move& a, const Move& b) {
        return a.score > b.score;
    });
    
    if (possibleMoves.size() > 14) {
        possibleMoves.resize(14); 
    }

    int alpha = -INF;
    int beta = INF;

    for (auto& move : possibleMoves) {
        board.makeMove(move.row, move.col, aiPlayer);
        int moveScore = alphaBeta(board, depth - 1, alpha, beta, false, move.row, move.col, aiPlayer);
        board.undoMove(move.row, move.col);

        if (noiseRange > 0) {
            moveScore += (std::rand() % noiseRange) - (noiseRange / 2);
        }

        if (moveScore > bestMove.score || (moveScore == bestMove.score && move.score > bestHeuristicScore)) {
            bestMove.score = moveScore;
            bestMove.row = move.row;
            bestMove.col = move.col;
            bestHeuristicScore = move.score;
        }
        alpha = std::max(alpha, moveScore);
    }
    return bestMove;
}