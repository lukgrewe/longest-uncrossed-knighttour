#ifndef UNCROSSEDKNIGHTTOUR_HELPER_H
#define UNCROSSEDKNIGHTTOUR_HELPER_H
#include <bitset>

struct Position { int x, y; };

typedef std::bitset<8> Byte;

void printWelcomeScreen() {
    std::cout << "  _    _                                       _   _  __      _       _     _                            " << std::endl;
    std::cout << " | |  | |                                     | | | |/ /     (_)     | |   | |                           " << std::endl;
    std::cout << " | |  | |_ __   ___ _ __ ___  ___ ___  ___  __| | | ' / _ __  _  __ _| |__ | |_ ___  _   _ _ __          " << std::endl;
    std::cout << " | |  | | '_ \\ / __| '__/ _ \\/ __/ __|/ _ \\/ _` | |  < | '_ \\| |/ _` | '_ \\| __/ _ \\| | | | '__|   " << std::endl;
    std::cout << " | |__| | | | | (__| | | (_) \\__ \\__ \\  __/ (_| | | . \\| | | | | (_| | | | | || (_) | |_| | |        " << std::endl;
    std::cout << "  \\____/|_| |_|\\___|_|  \\___/|___/___/\\___|\\__,_| |_|\\_\\_| |_|_|\\__, |_| |_|\\__\\___/ \\__,_|_| " << std::endl;
    std::cout << "______________________________________________________________     / |   ________________________        " << std::endl;
    std::cout << "                                                                 |___/                                   " << std::endl;
}

#endif //UNCROSSEDKNIGHTTOUR_HELPER_H
