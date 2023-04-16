cmake_minimum_required(VERSION 3.22)
project(UncrossedKnightTour)

set(CMAKE_CXX_FLAGS_RELEASE "-march=native -O3")
set(CMAKE_CXX_STANDARD 23)

add_executable(UncrossedKnightTour
        main.cpp
        classes/Board.h
        classes/KnightMoveTask_seq.h
        classes/Helper.h
        classes/KnightMoveTask_par.h)

find_package(TBB)
target_link_libraries(UncrossedKnightTour TBB::tbb)