#include <iostream>
#include "ui/Menu.h"

int main() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    Menu app;
    app.run();
    return 0;
}