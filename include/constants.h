#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <array>
#include <cstdint>

constexpr std::array<std::uint64_t, 64> generate_queen_attacks() {
    std::array<std::uint64_t, 64> attacks{};

    for (int sq = 0; sq < 64; ++sq) {
        const int r = sq / 8;
        const int f = sq % 8;
        std::uint64_t mask = 0;

        // 1. Horizontal (Rank) and Vertical (File) attacks
        for (int i = 0; i < 8; ++i) {
            mask |= (1ULL << (r * 8 + i));
            mask |= (1ULL << (i * 8 + f));
        }

        // 2. Diagonal and Anti-Diagonal attacks
        for (int i = r + 1, j = f + 1; i < 8 && j < 8; ++i, ++j) mask |= (1ULL << (i * 8 + j));
        for (int i = r - 1, j = f - 1; i >= 0 && j >= 0; --i, --j) mask |= (1ULL << (i * 8 + j));
        for (int i = r + 1, j = f - 1; i < 8 && j >= 0; ++i, --j) mask |= (1ULL << (i * 8 + j));
        for (int i = r - 1, j = f + 1; i >= 0 && j < 8; --i, ++j) mask |= (1ULL << (i * 8 + j));

        // A queen does not attack its own square, so we remove it from the mask.
        mask &= ~(1ULL << sq);

        attacks[sq] = mask;
    }

    return attacks;
}

inline constexpr auto QUEEN_ATTACKS = generate_queen_attacks();


#endif