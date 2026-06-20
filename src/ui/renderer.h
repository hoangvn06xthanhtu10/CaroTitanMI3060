#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "../core/Board.h"

class Renderer {
private:
    float cellSize;

public:
    Renderer(float size = 40.0f); // Mỗi ô cờ rộng 40x40 pixel
    void drawBoard(sf::RenderWindow& window);
    void drawPieces(sf::RenderWindow& window, const Board& board);
};

#endif