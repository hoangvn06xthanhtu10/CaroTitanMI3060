#include "Board.h"
#include <algorithm>

Board::Board() { reset(); }

void Board::reset() {
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) cells[i] = EMPTY;
}

int Board::getCell(int r, int c) const { return cells[r * BOARD_SIZE + c]; }

void Board::makeMove(int r, int c, int player) { cells[r * BOARD_SIZE + c] = player; }

void Board::undoMove(int r, int c) { cells[r * BOARD_SIZE + c] = EMPTY; }

bool Board::isValidMove(int r, int c) const {
    return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && getCell(r, c) == EMPTY;
}

// Kiểm tra 5 quân liên tiếp từ nước đi mới nhất (Tối ưu O(1) thay vì quét cả bàn cờ)
bool Board::checkWin(int r, int c, int player) const {
    int dr[] = {0, 1, 1, 1}; // Ngang, Dọc, Chéo xuôi, Chéo ngược
    int dc[] = {1, 0, 1, -1};
    int opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;

    for (int i = 0; i < 4; ++i) {
        int count = 1;
        int blocks = 0; // Đếm số đầu bị chặn (Tối đa là 2)

        // 1. Quét về phía trước
        int nr = r + dr[i], nc = c + dc[i];
        while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && getCell(nr, nc) == player) {
            count++; 
            nr += dr[i]; 
            nc += dc[i];
        }
        // Kiểm tra xem đầu phía trước có bị chặn bởi mép bàn cờ hoặc quân địch không
        if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || getCell(nr, nc) == opponent) {
            blocks++;
        }

        // 2. Quét về phía sau
        nr = r - dr[i]; 
        nc = c - dc[i];
        while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && getCell(nr, nc) == player) {
            count++; 
            nr -= dr[i]; 
            nc -= dc[i];
        }
        // Kiểm tra xem đầu phía sau có bị chặn không
        if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || getCell(nr, nc) == opponent) {
            blocks++;
        }

        // ĐIỀU KIỆN THẮNG CHUẨN CARO: Đủ 5 quân và KHÔNG bị chặn cả 2 đầu
        if (count >= 5 && blocks < 2) {
            return true;
        }
    }
    return false;
}

// Tìm các ô trống có bán kính 1-2 ô quanh các quân cờ đã đánh (Giúp Minimax không phải duyệt toàn bộ ô trống)
std::vector<Move> Board::getGeneratorMoves() const {
    std::vector<Move> moves;
    bool visited[BOARD_SIZE * BOARD_SIZE] = {false};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (getCell(r, c) != EMPTY) {
                // Quét vùng lân cận 1-2 ô
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int nr = r + i, nc = c + j;
                        if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && getCell(nr, nc) == EMPTY) {
                            if (!visited[nr * BOARD_SIZE + nc]) {
                                visited[nr * BOARD_SIZE + nc] = true;
                                moves.push_back({nr, nc, 0});
                            }
                        }
                    }
                }
            }
        }
    }
    // Nếu bàn cờ trống, ưu tiên đánh vào tâm bàn cờ
    if (moves.empty()) moves.push_back({BOARD_SIZE / 2, BOARD_SIZE / 2, 0});
    return moves;
}
// Hàm tìm kiếm các nước đi bắt buộc phải chặn (Địch có 4 hoặc 3 quân)
std::vector<Move> Board::getForcedMoves(int player) {
    std::vector<Move> forcedMoves;
    int opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;

    int dr[] = {0, 1, 1, 1}; // Ngang, Dọc, Chéo xuôi, Chéo ngược
    int dc[] = {1, 0, 1, -1};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (getCell(r, c) == EMPTY) {
                
                // Giả định nếu Đối thủ đánh vào ô trống này
                makeMove(r, c, opponent);

                // NGUY HIỂM CẤP 1: Nước đi giúp đối thủ THẮNG NGAY
                if (checkWin(r, c, opponent)) {
                    forcedMoves.push_back({r, c, 1000000}); // Ưu tiên cao nhất
                    undoMove(r, c);
                    continue; 
                }

                // NGUY HIỂM CẤP 2: Nước đi giúp đối thủ tạo thành thế OPEN 4 (Trống 2 đầu)
                for (int i = 0; i < 4; ++i) {
                    int count = 1;
                    
                    // Điểm tuần tra tiến về phía trước
                    int fr = r + dr[i], fc = c + dc[i];
                    while (fr >= 0 && fr < BOARD_SIZE && fc >= 0 && fc < BOARD_SIZE && getCell(fr, fc) == opponent) {
                        count++; fr += dr[i]; fc += dc[i];
                    }

                    // Điểm tuần tra lùi về phía sau
                    int br = r - dr[i], bc = c - dc[i];
                    while (br >= 0 && br < BOARD_SIZE && bc >= 0 && bc < BOARD_SIZE && getCell(br, bc) == opponent) {
                        count++; br -= dr[i]; bc -= dc[i];
                    }

                    // Nếu nước đi này tạo thành chuỗi đúng 4 quân liên tiếp
                    if (count == 4) {
                        // Kiểm tra xem hai đầu của chuỗi 4 quân này có THỰC SỰ TRỐNG không
                        bool fOpen = (fr >= 0 && fr < BOARD_SIZE && fc >= 0 && fc < BOARD_SIZE && getCell(fr, fc) == EMPTY);
                        bool bOpen = (br >= 0 && br < BOARD_SIZE && bc >= 0 && bc < BOARD_SIZE && getCell(br, bc) == EMPTY);
                        
                        if (fOpen && bOpen) {
                            forcedMoves.push_back({r, c, 500000}); // Ưu tiên cấp bách: Chặn đứng Open 4!
                            break; 
                        }
                    }
                }

                undoMove(r, c); // Hoàn tác trạng thái giả lập
            }
        }
    }

    // Sắp xếp danh sách nguy hiểm: Ưu tiên chặn nước Thắng trước, chặn nước Open 4 sau
    std::sort(forcedMoves.begin(), forcedMoves.end(), [](const Move& a, const Move& b) {
        return a.score > b.score;
    });

    return forcedMoves;
}