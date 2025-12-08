#include <iostream>
#include "ui/Menu.h"

int main() {
    std::cout << "PROGRAM STARTED\n";   // <--- DEBUG CEK
    Menu app;
    std::cout << "MENU LOADED\n";       // <--- DEBUG CEK
    app.mainLoop();
    std::cout << "PROGRAM EXIT\n";
    return 0;
}
