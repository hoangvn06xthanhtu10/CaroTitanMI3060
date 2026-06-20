#include "renderer.h"

Renderer::Renderer(float size) : cellSize(size) {}

void Renderer::drawBoard(sf::RenderWindow& window) {
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        sf::Vertex lineH[] = {
            {{0.f, i * cellSize}, sf::Color(60, 60, 60)},
            {{BOARD_SIZE * cellSize, i * cellSize}, sf::Color(60, 60, 60)}
        };
        window.draw(lineH, 2, sf::PrimitiveType::Lines);

        sf::Vertex lineV[] = {
            {{i * cellSize, 0.f}, sf::Color(60, 60, 60)},
            {{i * cellSize, BOARD_SIZE * cellSize}, sf::Color(60, 60, 60)}
        };
        window.draw(lineV, 2, sf::PrimitiveType::Lines);
    }
}

void Renderer::drawPieces(sf::RenderWindow& window, const Board& board) {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int cellState = board.getCell(r, c);
            
            // Tính toán tâm của ô cờ để đặt quân cờ vào chính giữa
            float centerX = c * cellSize + cellSize / 2.f;
            float centerY = r * cellSize + cellSize / 2.f;

            if (cellState == PLAYER_X) {
                // VẼ QUÂN X (MÀU XANH NEON)
                float length = cellSize - 16.f; // Chiều dài 2 nét gạch
                float thickness = 3.f;          // Độ dày nét vẽ

                // Nét chéo thứ nhất (\)
                sf::RectangleShape line1({length, thickness});
                line1.setFillColor(sf::Color::Cyan);
                line1.setOrigin({length / 2.f, thickness / 2.f}); // Đẩy tâm xoay vào giữa nét
                line1.setPosition({centerX, centerY});
                line1.setRotation(sf::degrees(45.f)); // Chuẩn SFML 3.0

                // Nét chéo thứ hai (/)
                sf::RectangleShape line2({length, thickness});
                line2.setFillColor(sf::Color::Cyan);
                line2.setOrigin({length / 2.f, thickness / 2.f});
                line2.setPosition({centerX, centerY});
                line2.setRotation(sf::degrees(-45.f)); // Chuẩn SFML 3.0

                window.draw(line1);
                window.draw(line2);
            } 
            else if (cellState == PLAYER_O) {
                // VẼ QUÂN O (MÀU ĐỎ)
                float radius = cellSize / 2.f - 8.f; 
                sf::CircleShape oShape(radius);
                oShape.setOutlineColor(sf::Color::Red);
                oShape.setOutlineThickness(3.f);
                oShape.setFillColor(sf::Color::Transparent);
                oShape.setOrigin({radius, radius}); // Đẩy tâm hình tròn vào giữa
                oShape.setPosition({centerX, centerY});
                
                window.draw(oShape);
            }
        }
    }
}