#ifndef MENU_WINDOWS_H
#define MENU_WINDOWS_H

#include <SFML/Graphics.hpp>

class MenuWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    int selectedSide;
    int selectedLevel;
    bool isGameStarted;

    sf::FloatRect btnX, btnO, btnMinus, btnPlus, btnPlay;

    void drawText(const std::string& str, float x, float y, int size, sf::Color color, bool center = true);
    void drawButton(sf::FloatRect bounds, sf::Color color, const std::string& textStr);
    void processEvents();
    void render();

public:
    MenuWindow();
    bool run(int& outSide, int& outLevel);
};

#endif