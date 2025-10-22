#include "../include/generate.h"

#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

std::uint64_t generate_initial_bitboard(std::mt19937& gen) {
    std::uniform_int_distribution<int> row_dist(0, 7);
    std::uint64_t bitboard = 0;

    for (int col = 0; col < 8; ++col) {
        int row = row_dist(gen);
        int pos = row * 8 + col;            // adjust if your mapping is different
        bitboard |= (1ULL << pos);
    }
    return bitboard;
}
