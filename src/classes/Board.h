#ifndef UNCROSSEDKNIGHTTOUR_BOARD_H
#define UNCROSSEDKNIGHTTOUR_BOARD_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>

#include "Helper.h"

extern int width, height;

class Board {
public:
    /**
     * Description of bit in tiles are set:
     * ( First direction 2 jumps, second direction 1 jump )
     * 00000000 - UpLeft⁷, UpRight⁶, RightUp⁵, RightDown⁴, DownRight³, DownLeft², LeftDown¹, LeftUp⁰
     * */
    Byte **tiles_a;

    Board() {
        tiles_a = new Byte*[width];
        for(int x = 0; x < width; ++x)
            tiles_a[x] = new Byte[height];
    };

    Board(Board &b) {
        tiles_a = new Byte*[width];
        for(int x = 0; x < width; ++x)
            tiles_a[x] = new Byte[height];

        for(int x = 0; x < width; x++)
            std::memcpy(tiles_a[x], b.tiles_a[x], height*sizeof(Byte));

    };

    Board(const Board &b) {
        tiles_a = new Byte*[width];
        for(int x = 0; x < width; ++x)
            tiles_a[x] = new Byte[height];

        for(int x = 0; x < width; x++)
            std::memcpy(tiles_a[x], b.tiles_a[x], height*sizeof(Byte));

    };

   ~Board(){
       /// free memory
       for(int x = 0; x < width; ++x) {
           delete[] tiles_a[x];
       }
       delete[] tiles_a;
   }

    void initCleanBoard() {
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                tiles_a[x][y] = Byte(0b00000000);
            }
        }
    }

    void printVisitedTiles() {
        std::cout << std::endl << "Board state:";
        for (int y = height-1; y >= 0; y--) {
            std::cout << std::endl;
            for (int x = 0; x < width; x++) {
                std::cout << tiles_a[x][y].any() << "  ";
            }
        }
        std::cout << std::endl << std::endl;
    }
};

#endif //UNCROSSEDKNIGHTTOUR_BOARD_H
