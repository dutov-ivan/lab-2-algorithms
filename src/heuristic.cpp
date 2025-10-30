#include "../include/heuristic.h"
#include <cstdint>
#include "common.h"
#include "../include/masks.h"

unsigned int CountAttackingPairs::calculate(const Board &board) {
    int total_attacks = 0;

    Board temp_board = board;

    for (const auto sq: board) {
        temp_board.clear_queen(sq);
        total_attacks += temp_board.queens_attacked_from(sq).count_queens();
    }

    return total_attacks;
}

unsigned int LineOccupancyHeuristic::calculate(const Board &board) {
    const uint64_t bb = board.get();
    int h = 0;

    auto add_conflicts = [&](const uint64_t mask) {
        if (const int occ = std::popcount(bb & mask); occ > 1) h += occ - 1;
    };

    for (const auto m: ROW_MASKS) add_conflicts(m);
    for (const auto m: COL_MASKS) add_conflicts(m);
    for (const auto m: MAIN_DIAG_MASKS) add_conflicts(m);
    for (const auto m: ANTI_DIAG_MASKS) add_conflicts(m);

    return h;
}
