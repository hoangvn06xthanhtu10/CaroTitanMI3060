#include "heuristic.h"
#include <cmath>
#include <algorithm>

// Hàm chấm điểm cho một nhóm 5 ô liên tiếp
int evaluateWindow(int countX, int countO) {
    // Nếu ô bị lẫn cả quân X và quân O thì không bên nào tạo được 5 quân ở đây -> 0 điểm
    if (countX > 0 && countO > 0) return 0; 

    // --- ĐIỂM CHO AI (PLAYER_O) - Cần tối đa hóa ---
    if (countO == 5) return 100000; // AI thắng
    if (countO == 4) return 10000;  // AI có thế 4 quân mở
    if (countO == 3) return 1000;   // AI có thế 3 quân
    if (countO == 2) return 100;
    if (countO == 1) return 10;

    // --- ĐIỂM CỦA ĐỊCH (PLAYER_X) - AI nhìn thấy điểm âm lớn sẽ ưu tiên ĐÁNH CHẶN ---
    if (countX == 5) return -100000;
    if (countX == 4) return -30000; // Điểm phạt cực nặng, ép AI phải bỏ công kích để vào chặn Địch ngay
    if (countX == 3) return -3000;  // Chặn thế 3 từ sớm
    if (countX == 2) return -50;
    if (countX == 1) return -5;

    return 0;
}

int Heuristic::evaluateBoard(const Board& board) {
    int totalScore = 0;

    // 1. Quét theo các Hàng Ngang
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c <= BOARD_SIZE - 5; ++c) {
            int countX = 0, countO = 0;
            for (int i = 0; i < 5; ++i) {
                int val = board.getCell(r, c + i);
                if (val == PLAYER_X) countX++;
                else if (val == PLAYER_O) countO++;
            }
            totalScore += evaluateWindow(countX, countO);
        }
    }

    // 2. Quét theo các Hàng Dọc
    for (int c = 0; c < BOARD_SIZE; ++c) {
        for (int r = 0; r <= BOARD_SIZE - 5; ++r) {
            int countX = 0, countO = 0;
            for (int i = 0; i < 5; ++i) {
                int val = board.getCell(r + i, c);
                if (val == PLAYER_X) countX++;
                else if (val == PLAYER_O) countO++;
            }
            totalScore += evaluateWindow(countX, countO);
        }
    }

    // 3. Quét theo Đường Chéo Xuôi (\)
    for (int r = 0; r <= BOARD_SIZE - 5; ++r) {
        for (int c = 0; c <= BOARD_SIZE - 5; ++c) {
            int countX = 0, countO = 0;
            for (int i = 0; i < 5; ++i) {
                int val = board.getCell(r + i, c + i);
                if (val == PLAYER_X) countX++;
                else if (val == PLAYER_O) countO++;
            }
            totalScore += evaluateWindow(countX, countO);
        }
    }

    // 4. Quét theo Đường Chéo Ngược (/)
    for (int r = 0; r <= BOARD_SIZE - 5; ++r) {
        for (int c = 4; c < BOARD_SIZE; ++c) {
            int countX = 0, countO = 0;
            for (int i = 0; i < 5; ++i) {
                int val = board.getCell(r + i, c - i);
                if (val == PLAYER_X) countX++;
                else if (val == PLAYER_O) countO++;
            }
            totalScore += evaluateWindow(countX, countO);
        }
    }

    // Tích hợp nhẹ trọng số tâm để AI định hình thế trận co cụm ban đầu tốt hơn
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board.getCell(r, c) != EMPTY) {
                int centerBonus = (7 - std::abs(7 - r)) + (7 - std::abs(7 - c));
                totalScore += (board.getCell(r, c) == PLAYER_O) ? centerBonus : -centerBonus;
            }
        }
    }

    return totalScore;
}