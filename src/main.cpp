#include <iostream>
#include <chrono>

#include "classes/KnightMoveTask_seq.h"
#include "classes/KnightMoveTask_par.h"
#include "classes/Helper.h"
#include "classes/Board.h"

/// global variables
int width, height, useParallelAlgorithm = 0;
std::vector<Position> startPositions;

/// function declaration
void initStartPositions();

void startSequentialAlgorithm(Board &board);
void writeSeqTourToConsole();
void writeSeqTourToFile();
void writeParTourToConsole();

void startParallelAlgorithm(Board &board);

int main() {
    printWelcomeScreen();
    std::cout << "Enter board width: ";
    std::cin >> width;
    std::cout << "Enter board height: ";
    std::cin >> height;
    std::cout << "Parallel algorithm? (0/1): ";
    std::cin >> useParallelAlgorithm;

    /// create chessboard
    Board board = Board();
    board.initCleanBoard();

    /// init start positions
    initStartPositions();

    if(useParallelAlgorithm == 0)
        startSequentialAlgorithm(board);
    else
        startParallelAlgorithm(board);

    return 0;
}

void startSequentialAlgorithm(Board &board){
    /// start timer
    auto startTime = std::chrono::high_resolution_clock::now();

    /// start algorithm for every start position
    for(int i = 0; i < startPositions.size(); i++) {
        Board b = Board(board);
        KnightMoveTaskSeq(b, 0, NULL,startPositions[i].x, startPositions[i].y)
                .doKnightMove(startPositions[i].x, startPositions[i].y);
    }

    /// snap ending time
    auto endTime = std::chrono::high_resolution_clock::now();

    /// print finished
    writeSeqTourToConsole();
    writeSeqTourToFile();

    /// calc and print duration of algorithm
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "Time to complete: ";
    std::cout << duration.count() << "us ~ "; // print microseconds
    std::cout << float(duration.count())/float(1000) << "ms ~ "; // calc and print milliseconds
    std::cout << float(duration.count())/float(1000000) << "s" << std::endl; // calc and print seconds
}

void startParallelAlgorithm(Board &board) {
    /// init one tbb
    auto num_threads = std::thread::hardware_concurrency();
    int defaultThreads = oneapi::tbb::info::default_concurrency();
    oneapi::tbb::global_control global_limit(
            oneapi::tbb::global_control::max_allowed_parallelism,
            num_threads > 0 ? num_threads : defaultThreads);

    oneapi::tbb::task_group g;
    /// start timer
    auto startTime = std::chrono::high_resolution_clock::now();

    /// start algorithm for every start position
    for(int i = 0; i < startPositions.size(); i++) {
        Board b = Board(board);
        std::vector<Position> tour{};
        KnightMoveTaskPar knt = KnightMoveTaskPar(g, board, 0, tour, startPositions[i].x, startPositions[i].y, startPositions[i].x, startPositions[i].y);
        g.run(knt);
    }
    g.wait();

    /// snap ending time
    auto endTime = std::chrono::high_resolution_clock::now();

    /// print finished
    writeParTourToConsole();
    writeParTourToFile();

    /// calc and print duration of algorithm
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "Time to complete: ";
    std::cout << duration.count() << "us ~ "; // print microseconds
    std::cout << float(duration.count())/float(1000) << "ms ~ "; // calc and print milliseconds
    std::cout << float(duration.count())/float(1000000) << "s" << std::endl; // calc and print seconds
}

void initStartPositions() {
    int w, h;

    if(width%2 == 0) w = width/2;
    else w = width/2 + 1;

    if(height%2 == 0) h = height/2;
    else h = height/2 + 1;

    if(width == height) { /// square board
        for(int i = 0; i < w; i++) {
            for(int k = i; k < h; k++) {
                Position p;
                p.x = i;
                p.y = k;
                startPositions.push_back(p);
            }
        }
    } else { /// rectangle board
        for(int i = 0; i < w; i++) {
            for(int k = 0; k < h; k++) {
                Position p;
                p.x = i;
                p.y = k;
                startPositions.push_back(p);
            }
        }
    }
}