#include "game_windows.h"
#include <iostream>
#include <optional>

// Tiếp nhận cấu hình và khởi tạo trạng thái ban đầu
GameWindow::GameWindow(int chosenSide, int level) 
    : cellSize(40.0f), currentTurn(PLAYER_X), isGameOver(false), turnCount(1), 
      humanSide(chosenSide), aiLevel(level) 
{
    aiSide = (humanSide == PLAYER_X) ? PLAYER_O : PLAYER_X;
    
    unsigned int sizePx = BOARD_SIZE * static_cast<unsigned int>(cellSize);
    window.create(sf::VideoMode({sizePx, sizePx}), "CaroTitan AI - Level " + std::to_string(aiLevel));
    window.setFramerateLimit(60);
    board.reset();
}

void GameWindow::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void GameWindow::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // CHỈ xử lý click chuột nếu ĐANG LÀ LƯỢT CỦA CON NGƯỜI
        if (!isGameOver && currentTurn == humanSide) {
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    int c = mousePressed->position.x / cellSize;
                    int r = mousePressed->position.y / cellSize;

                    if (board.isValidMove(r, c)) {
                        board.makeMove(r, c, humanSide);
                        std::cout << turnCount << ". Player (" << (humanSide == PLAYER_X ? "X" : "O") 
                                  << "): (" << r << "," << c << ")\n";
                        
                        if (board.checkWin(r, c, humanSide)) {
                            std::cout << "MATCH OVER: " << (humanSide == PLAYER_X ? "X" : "O") << " wins!\n";
                            isGameOver = true;
                        } else {
                            currentTurn = aiSide; // Chuyển lượt sang cho AI
                            turnCount++;
                        }
                    }
                }
            }
        }
    }
}

void GameWindow::update() {
    // CHỈ xử lý nếu ĐANG LÀ LƯỢT CỦA AI
    if (!isGameOver && currentTurn == aiSide) {
        // Truyền AI Level và quân cờ của AI vào hàm tính toán nước đi
        Move aiMove = ai.findBestMove(board, aiLevel, aiSide);
        
        if (aiMove.row != -1 && aiMove.col != -1) {
            board.makeMove(aiMove.row, aiMove.col, aiSide);
            std::cout << turnCount << ". AI (" << (aiSide == PLAYER_X ? "X" : "O") 
                      << "): (" << aiMove.row << "," << aiMove.col << ")\n";

            if (board.checkWin(aiMove.row, aiMove.col, aiSide)) {
                std::cout << "MATCH OVER: " << (aiSide == PLAYER_X ? "X" : "O") << " wins!\n";
                isGameOver = true;
            } else {
                currentTurn = humanSide; // Trả lượt về cho con người
                turnCount++;
            }
        }
    }
}

void GameWindow::render() {
    window.clear(sf::Color(25, 25, 25));
    renderer.drawBoard(window);
    renderer.drawPieces(window, board);
    window.display();
}