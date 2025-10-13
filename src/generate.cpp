#include "../include/generate.h"

#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

std::uint64_t generate_initial_bitboard(std::mt19937& gen)
{
    std::vector<int> positions(64);
    std::iota(positions.begin(), positions.end(), 0);

    std::shuffle(positions.begin(), positions.end(), gen);

    std::uint64_t bitboard = 0;
    for (int i = 0; i < 8; ++i) {
        bitboard |= (1ULL << positions[i]);
    }
    return bitboard;
}
