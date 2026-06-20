#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include "../core/board.h"
#include "../core/minimax.h"
#include "renderer.h"

class GameWindow {
private:
    sf::RenderWindow window;
    Board board;
    Renderer renderer;
    Minimax ai;
    
    int currentTurn;
    bool isGameOver;
    float cellSize;
    int turnCount;

    // Thay đổi: Thêm biến lưu cấu hình trận đấu
    int humanSide;
    int aiLevel;
    int aiSide;

    void processEvents();
    void update();
    void render();

public:
    // Thay đổi: Constructor tiếp nhận lựa chọn từ Main
    GameWindow(int chosenSide, int level);
    void run();
};

#endif