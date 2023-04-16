#ifndef UNCROSSEDKNIGHTTOUR_KNIGHTMOVETASK_SEQ_H
#define UNCROSSEDKNIGHTTOUR_KNIGHTMOVETASK_SEQ_H

#include <vector>
#include <iostream>
#include <fstream>

#include "Board.h"
#include "Helper.h"

extern int width, height; /// global set width and height

Position *longestTour = new Position[100];
Board *longestTourBoard;
int deepestDepth = -1;

class KnightMoveTaskSeq {
public:
    int x_start, y_start, depth;
    Board b;
    Position *tour;

    /**
     * Contructor - init object and allocate memory
     *
     * @param b - current chessboard
     * @param depth - current depth
     * @param prev_tour - previous tour coordinates
     * @param x_start - start width of tour
     * @param y_start - start height of tour
     */
    KnightMoveTaskSeq(Board &b, int depth, Position *prev_tour, int &x_start, int &y_start)
        : b(b), depth(depth), x_start(x_start), y_start(y_start) {
        tour = new Position[depth + 1];
        if (prev_tour != NULL)
            std::memcpy(tour, prev_tour, (depth + 1) * sizeof(Position));
    };

    /**
     * Destructor - delete dynamic memory
     */
    ~KnightMoveTaskSeq() { delete[] tour; }

    /**
     * Backtracking function, to find longest uncrossed Knight Move on an n*m chessboard
     *
     * @param x - current width board position
     * @param y - current height position board
     */
    void doKnightMove(int x, int y) {
        tour[depth].x = x; /// add current tile to tour
        tour[depth].y = y;

        if (depth > 0 && x == x_start && y == y_start)  { /// found new knight tour
            if(depth > deepestDepth) { /// set longest tour
                std::memcpy(longestTour, tour, (depth + 1) * sizeof(Position));
                deepestDepth = depth;
                longestTourBoard = new Board(b);
            }
            return;
        }

        if (b.tiles_a[x][y][7] == 0 && jumpAllowed_UpperLeft(x, y))
            ///  do jump UpLeft - mark UpLeft on this tile and DownRight on dest tile
            knightJumpsDeeper(0b10000000, 0b00001000, x, y, x - 1, y + 2);

        if (b.tiles_a[x][y][6] == 0 && jumpAllowed_UpperRight(x, y))
            /// do jump 2 up 1 right - mark UpRight on this tile and DownLeft on dest tile
            knightJumpsDeeper(0b01000000, 0b00000100, x, y, x + 1, y + 2);

        if (b.tiles_a[x][y][5] == 0 && jumpAllowed_RightUp(x, y))
            /// do jump 2 right 1 up - mark RightUp on this tile and LeftDown on dest tile
            knightJumpsDeeper(0b00100000, 0b00000010, x, y, x + 2, y + 1);

        if (b.tiles_a[x][y][4] == 0 && jumpAllowed_RightDown(x, y))
            /// do jump 2 right 1 down - mark RightDown on this tile and LeftUp on dest tile
            knightJumpsDeeper(0b00010000, 0b00000001, x, y, x + 2, y - 1);

        if (b.tiles_a[x][y][3] == 0 && jumpAllowed_DownRight(x, y))
            /// do jump 2 down 1 right - mark DownRight on this tile and UpLeft on dest tile
            knightJumpsDeeper(0b00001000, 0b10000000, x, y, x + 1, y - 2);

        if (b.tiles_a[x][y][2] == 0 && jumpAllowed_DownLeft(x, y))
            /// do jump 2 down 1 left - mark DownLeft on this tile and UpRight on dest tile
            knightJumpsDeeper(0b00000100, 0b01000000, x, y, x - 1, y - 2);

        if (b.tiles_a[x][y][1] == 0 && jumpAllowed_LeftDown(x, y))
            /// do jump 2 left 1 down - mark LeftDown on this tile and RightUp on dest tile
            knightJumpsDeeper(0b00000010, 0b00100000, x, y, x - 2, y - 1);

        if (b.tiles_a[x][y][0] == 0 && jumpAllowed_LeftUp(x, y))
            /// do jump 2 left 1 up - mark LeftUp on this tile and RightDown on dest tile
            knightJumpsDeeper(0b00000001, 0b00010000, x, y, x - 2, y + 1);
    }

    /**
     * Function to execute a Knight-Jump. It copies current board, marks tiles/jumps and
     * starting the backtracking for a next depth.
     *
     * @param outgoingJump - marker byte to set jump on this tile
     * @param ingoingJump - marker byte to set jump on destination tile
     * @param x - current x position
     * @param y - current y position
     * @param dest_x - destination x position
     * @param dest_y - destination y position
     */
    void knightJumpsDeeper(const Byte &outgoingJump, const Byte &ingoingJump,
                           const int &x, const int &y, const int &dest_x, const int &dest_y) {
        Board nb = Board(b);
        nb.tiles_a[x][y] |= outgoingJump; // set LeftUp jump
        nb.tiles_a[dest_x][dest_y] |= ingoingJump; // set RightDown jump

        KnightMoveTaskSeq knt = KnightMoveTaskSeq(nb, depth + 1, tour, x_start, y_start);
        knt.doKnightMove(dest_x, dest_y);
    }

    /***
     * POSSIBLE KNIGHT MOVES CHECK
     * #####################################################################################
     *
     * Below functions checking if a direction specific jump is allowed. To do this every
     * function uses its own crossing szenario and checks all possible 9 crossings for its
     * direction.
     */

    /// ########## 2 up 1 left
    bool jumpAllowed_UpperLeft (int &x, int &y) {
        if(x-1 < 0 || y+3 > height) return false; // out of bounds
        if(b.tiles_a[x-1][y+2].any() && !((x-1) == x_start && y+2 == y_start)) return false;

        Byte &yellow = b.tiles_a[x][y+1];
        if(yellow[0] || yellow[1] || yellow[2]) return false; // jumps LeftUp, LeftDown, DownLeft

        Byte &blue = b.tiles_a[x-1][y+1];
        if(blue[6] || blue[5] || blue[4]) return false; // jumps UpRight, RightUp, RightDown

        Byte &green = b.tiles_a[x][y+2];
        if(green[1] || green[2]) return false; // jumps LeftDown, DownLeft,

        Byte &purple = b.tiles_a[x-1][y];
        if(purple[5]) return false; // jumps RightUp

        return true;
    }

    /// ######### 2 up 1 right
    bool jumpAllowed_UpperRight (int &x, int &y) {
        if(x+2 > width || y+3 > height) return false;
        if(b.tiles_a[x+1][y+2].any() && !((x+1) == x_start && y+2 == y_start)) return false; // is visited?

        Byte &yellow = b.tiles_a[x][y+1];
        if(yellow[5] || yellow[4] || yellow[3]) return false; // jumps RightUp, RightDown, DownRight

        Byte &blue = b.tiles_a[x+1][y+1];
        if(blue[0] || blue[1] || blue[7]) return false; // jumps LeftUp, LeftDown, UpLeft

        Byte &green = b.tiles_a[x][y+2];
        if(green[4] || green[3]) return false; // jumps RightDown, DownRight

        Byte &purple = b.tiles_a[x+1][y];
        if(purple[0]) return false; // jump LeftUp

        return true;
    }

    /// ######### 2 right 1 up
    bool jumpAllowed_RightUp (int &x, int &y) {
        if(x+3 > width || y+2 > height) return false; // out of bounds
        if(b.tiles_a[x+2][y+1].any() && !((x+2) == x_start && y+1 == y_start)) return false; // is visited?

        Byte &yellow = b.tiles_a[x+1][y];
        if(yellow[0] || yellow[7] || yellow[6]) return false; // jumps LeftUp, UpLeft, UpRight

        Byte &blue = b.tiles_a[x+1][y+1];
        if(blue[4] || blue[3] || blue[2]) return false; // jumps RightDown, DownRight, DownLeft

        Byte &green = b.tiles_a[x+2][y];
        if(green[0] || green[7]) return false; // jumps LeftUp, UpLeft

        Byte &purple = b.tiles_a[x][y+1];
        if(purple[3]) return false; // jump DownRight

        return true;
    }

    /// ######### 2 right 1 down
    bool jumpAllowed_RightDown (int &x, int &y) {
        if(x+3 > width || y-1 < 0) return false; // out of bounds
        if(b.tiles_a[x+2][y-1].any() && !((x+2) == x_start && y-1 == y_start)) return false; // is visited?

        Byte &yellow = b.tiles_a[x+1][y];
        if(yellow[1] || yellow[2] || yellow[3]) return false; // jumps LeftDown, DownLeft, DownRight

        Byte &blue = b.tiles_a[x+1][y-1];
        if(blue[7] || blue[6] || blue[5]) return false; // jumps UpLeft, UpRight, RightUp

        Byte &green = b.tiles_a[x+2][y];
        if(green[2] || green[1]) return false; // jumps DownLeft, LeftDown

        Byte &purple = b.tiles_a[x][y-1];
        if(purple[6]) return false; // jumps UpRight

        return true;
    }

    /// ######### 2 down 1 right
    bool jumpAllowed_DownRight (int &x, int &y) {
        if(x+2 > width || y-2 < 0) return false; // out of bounds
        if(b.tiles_a[x+1][y-2].any() && !((x+1) == x_start && y-2 == y_start)) return false; // is visited?

        Byte &yellow = b.tiles_a[x][y-1];
        if(yellow[6] || yellow[5] || yellow[4]) return false; // jumps UpRight, RightUp, RightDown

        Byte &blue = b.tiles_a[x+1][y-1];
        if(blue[0] || blue[1] || blue[2]) return false; // jumps LeftUp, LeftDown, DownLeft

        Byte &green = b.tiles_a[x][y-2];
        if(green[6] || green[5]) return false; // jumps UpRight, RightUp

        Byte &purple = b.tiles_a[x+1][y];
        if(purple[1]) return false; // jumps LeftDown,

        return true;
    }

    /// ######### 2 down 1 left
    bool jumpAllowed_DownLeft (int &x, int &y) {
        if(x-1 < 0 || y-2 < 0) return false; // out of bounds
        if(b.tiles_a[x-1][y-2].any() && !((x-1) == x_start && y-2 == y_start)) return false; // visited

        Byte &yellow = b.tiles_a[x][y-1];
        if(yellow[7] || yellow[0]|| yellow[1]) return false; // jumps UpLeft, LeftUp, LeftDown

        Byte &blue = b.tiles_a[x-1][y-1];
        if(blue[5] || blue[4] || blue[3]) return false; // jumps RightUp, RightDown, DownRight

        Byte &green = b.tiles_a[x][y-2];
        if(green[7] || green[0]) return false; // jump UpLeft, LeftUp

        Byte &purple = b.tiles_a[x-1][y];
        if(purple[4]) return false; // jumps RightDown

        return true;
    }

    /// ######### 2 left 1 down
    bool jumpAllowed_LeftDown (int &x, int &y) {
        if(x-2 < 0 || y-1 < 0) return false; // out of bounds
        if(b.tiles_a[x-2][y-1].any() && !((x-2) == x_start && (y-1) == y_start)) return false; // is visited?

        Byte &yellow = b.tiles_a[x-1][y];
        if(yellow[2] || yellow[3] || yellow[4]) return false; // jumps DownLeft, DownRight, RightDown

        Byte &blue = b.tiles_a[x-1][y-1];
        if(blue[0] || blue[7] || blue[6]) return false; // jumps LeftUp, UpLeft, UpRight

        Byte &green = b.tiles_a[x-2][y];
        if(green[3] || green[4]) return false; // check jumps DownRight, RightDown,

        Byte &purple = b.tiles_a[x][y-1];
        if(purple[7]) return false; // jumps UpLeft

        return true;
    }

    /// ######### 2 left 1 up
    bool jumpAllowed_LeftUp (int &x, int &y) {
        if(x-2 < 0 || y+2 > height) return false; // out of bounds
        if(b.tiles_a[x-2][y+1].any() && !((x-2) == x_start && y+1 == y_start)) return false; // is visited?

        Byte &yellow = b.tiles_a[x-1][y];
        if(yellow[7] || yellow[6] || yellow[5]) return false; // jumps UpLeft, UpRight, RightUp

        Byte &blue = b.tiles_a[x-1][y+1];
        if(blue[2] || blue[3] || blue[1]) return false; // jumps DownLeft, DownRight, LeftDown

        Byte &green = b.tiles_a[x-2][y];
        if(green[6] || green[5]) return false; // jumps UpRight, RightUp

        Byte &purple = b.tiles_a[x][y+1];
        if(purple[2]) return false; // jump DownLeft

        return true;
    }
};

/**
 * Helper functions
 * #####################################################################################
 */

void writeSeqTourToConsole() {
    if(longestTourBoard != NULL) longestTourBoard->printVisitedTiles();
    std::cout << "Path: ";
    for(int i = 0; i < deepestDepth+1; i++) {
        if(i == deepestDepth)
            std::cout << "(" << longestTour[i].x << "," << longestTour[i].y << ") ";
        else
            std::cout << "(" << longestTour[i].x << "," << longestTour[i].y << ") -> ";
    }
    std::cout << std::endl << "Path length: " << deepestDepth << std::endl;
}

void writeSeqTourToFile() {
    std::ofstream file("./route.txt");
    for(int i = 0; i < deepestDepth+1; i++) {
        file << longestTour[i].x << "," << longestTour[i].y << std::endl;
    }
    file.close();
}

#endif //UNCROSSEDKNIGHTTOUR_KNIGHTMOVETASK_SEQ_H