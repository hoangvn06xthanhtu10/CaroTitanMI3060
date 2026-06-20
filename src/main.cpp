#include "ui/game_windows.h"
#include "ui/menu_windows.h"
#include <iostream>

int main() {
    // Ẩn Console trên Windows khi chạy game (Tùy chọn)
    // #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

    int chosenSide = 1;
    int aiLevel = 5;

    // 1. Khởi chạy Menu Đồ Họa
    MenuWindow menu;
    bool start = menu.run(chosenSide, aiLevel);

    // 2. Nếu người chơi nhấn "BẮT ĐẦU", khởi chạy Game
    if (start) {
        int player = (chosenSide == 1) ? PLAYER_X : PLAYER_O;
        std::cout << "Starting Game... Side: " << player << " | AI Level: " << aiLevel << "\n";
        
        GameWindow game(player, aiLevel);
        game.run();
    } else {
        std::cout << "Menu closed. Exiting game.\n";
    }

    return 0;
}