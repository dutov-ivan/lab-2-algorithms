#include "../include/generate.h"

#include <random>
#include <algorithm>

Board generate_initial_bitboard(std::mt19937 &gen) {
    std::uniform_int_distribution row_dist(0, 7);
    Board board;

    for (int col = 0; col < 8; ++col) {
        const int row = row_dist(gen);
        board.set_queen(queen_position(col, row));
    }
    return board;
}
