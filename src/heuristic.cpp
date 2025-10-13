#include "../include/heuristic.h"
#include <cstdint>
#include <array>
#include <iostream>
#include "../include/constants.h"

int count_attacking_pairs(uint64_t board) {
    int total_attacks = 0;

    while (board != 0) {
        // 1. Find the index of the least significant queen (0-63)
        int queen_index = __builtin_ctzll(board);

        // 2. Get the pre-computed attack mask for this queen
        uint64_t attack_mask = QUEEN_ATTACKS[queen_index];

        // 3. "Remove" this queen from the board for the next iteration's check
        board &= board - 1;

        // 4. Count how many of the REMAINING queens are on the attack mask
        // __builtin_popcountll is a GCC/Clang intrinsic for "Population Count"
        total_attacks += __builtin_popcountll(board & attack_mask);
    }

    return total_attacks;
}
