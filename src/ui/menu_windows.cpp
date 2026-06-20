#include "menu_windows.h"
#include <iostream>
#include <optional>

MenuWindow::MenuWindow() : selectedSide(1), selectedLevel(5), isGameStarted(false) {
    window.create(sf::VideoMode({600, 500}), "CaroTitan AI - Main Menu");
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/pixel.ttf")) {
        std::cerr << "Loi: Khong tim thay file font!\n";
    }

    // --- CẬP NHẬT CHUẨN SFML 3.0 ---
    // sf::FloatRect nhận cấu trúc {{x, y}, {width, height}}
    btnX = {{150.f, 150.f}, {120.f, 50.f}};
    btnO = {{330.f, 150.f}, {120.f, 50.f}};
    
    btnMinus = {{180.f, 300.f}, {50.f, 50.f}};
    btnPlus = {{370.f, 300.f}, {50.f, 50.f}};
    
    btnPlay = {{200.f, 400.f}, {200.f, 60.f}};
}

void MenuWindow::drawText(const std::string& str, float x, float y, int size, sf::Color color, bool center) {
    sf::Text text(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    
    if (center) {
        sf::FloatRect bounds = text.getLocalBounds();
        // SFML 3.0: Dùng .size.x và .size.y thay vì .width và .height
        text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }
    text.setPosition({x, y});
    window.draw(text);
}

void MenuWindow::drawButton(sf::FloatRect bounds, sf::Color color, const std::string& textStr) {
    // SFML 3.0 truyền thẳng bounds.size vào khối hình học
    sf::RectangleShape rect(bounds.size);
    rect.setPosition(bounds.position);
    rect.setFillColor(color);
    rect.setOutlineThickness(2.f);
    rect.setOutlineColor(sf::Color::White);
    window.draw(rect);

    // SFML 3.0: Dùng .position và .size để lấy tọa độ
    drawText(textStr, bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f - 5.f, 24, sf::Color::White);
}

void MenuWindow::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), 
                                      static_cast<float>(mousePressed->position.y));

                if (btnX.contains(mousePos)) selectedSide = 1;
                if (btnO.contains(mousePos)) selectedSide = 2;
                
                if (btnMinus.contains(mousePos) && selectedLevel > 1) selectedLevel--;
                if (btnPlus.contains(mousePos) && selectedLevel < 10) selectedLevel++;
                
                if (btnPlay.contains(mousePos)) {
                    isGameStarted = true;
                    window.close();
                }
            }
        }
    }
}

void MenuWindow::render() {
    window.clear(sf::Color(30, 30, 40)); 

    drawText("CAROTITAN AI", 300.f, 50.f, 40, sf::Color::Yellow);

    drawText("CHON PHE (1st Turn: X)", 300.f, 120.f, 20, sf::Color::White);
    drawButton(btnX, (selectedSide == 1) ? sf::Color(0, 150, 150) : sf::Color(100, 100, 100), "PHE X");
    drawButton(btnO, (selectedSide == 2) ? sf::Color(150, 0, 0) : sf::Color(100, 100, 100), "PHE O");

    drawText("CHON DO KHO AI", 300.f, 250.f, 20, sf::Color::White);
    drawButton(btnMinus, sf::Color(80, 80, 80), "-");
    drawButton(btnPlus, sf::Color(80, 80, 80), "+");
    drawText("Level " + std::to_string(selectedLevel), 300.f, 320.f, 30, sf::Color::Cyan);

    drawButton(btnPlay, sf::Color(0, 150, 50), "BAT DAU GAME");

    window.display();
}

bool MenuWindow::run(int& outSide, int& outLevel) {
    while (window.isOpen()) {
        processEvents();
        render();
    }
    
    outSide = selectedSide;
    outLevel = selectedLevel;
    return isGameStarted;
}