#include "../include/heuristic.h"
#include <cstdint>
#include "common.h"
#include "../include/masks.h"

double count_attacking_pairs(const Board &board) {
    int total_attacks = 0;

    Board temp_board = board;

    for (const auto sq: board) {
        temp_board.clear_queen(sq);
        total_attacks += temp_board.queens_attacked_from(sq).count_queens();
    }

    return total_attacks;
}

double line_occupancy_heuristic(const Board &b) {
    uint64_t bb = b.get();
    int h = 0;

    auto add_conflicts = [&](uint64_t mask) {
        int occ = std::popcount(bb & mask);
        if (occ > 1) h += (occ - 1);
    };

    for (auto m: ROW_MASKS) add_conflicts(m);
    for (auto m: COL_MASKS) add_conflicts(m);
    for (auto m: MAIN_DIAG_MASKS) add_conflicts(m);
    for (auto m: ANTI_DIAG_MASKS) add_conflicts(m);

    return h;
}
