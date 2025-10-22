#include "../include/heuristic.h"
#include <cstdint>
#include <array>
#include <iostream>

#include "common.h"
#include "../include/constants.h"

double count_attacking_pairs(const Board &board) {
    int total_attacks = 0;

    Board temp_board = board;

    for (const auto sq: board) {
        temp_board.clear_queen(sq);
        total_attacks += temp_board.queens_attacked_from(sq).count_queens();
    }

    return total_attacks;
}
