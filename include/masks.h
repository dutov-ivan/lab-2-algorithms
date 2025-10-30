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
            mask |= 1ULL << (r * 8 + i);
            mask |= 1ULL << (i * 8 + f);
        }

        // 2. Diagonal and Anti-Diagonal attacks
        for (int i = r + 1, j = f + 1; i < 8 && j < 8; ++i, ++j) mask |= 1ULL << (i * 8 + j);
        for (int i = r - 1, j = f - 1; i >= 0 && j >= 0; --i, --j) mask |= 1ULL << (i * 8 + j);
        for (int i = r + 1, j = f - 1; i < 8 && j >= 0; ++i, --j) mask |= 1ULL << (i * 8 + j);
        for (int i = r - 1, j = f + 1; i >= 0 && j < 8; --i, ++j) mask |= 1ULL << (i * 8 + j);

        // A queen does not attack its own square, so we remove it from the mask.
        mask &= ~(1ULL << sq);

        attacks[sq] = mask;
    }

    return attacks;
}

inline constexpr auto QUEEN_ATTACKS = generate_queen_attacks();

constexpr uint64_t coord_to_bit(const int r, const int c) {
    return 1ULL << (r * 8 + c);
}

// Row mask: all c = 0..7, fixed r
constexpr uint64_t row_mask(const int r) {
    uint64_t mask = 0;
    for (int c = 0; c < 8; ++c)
        mask |= coord_to_bit(r, c);
    return mask;
}

// Column mask: all r = 0..7, fixed c
constexpr uint64_t col_mask(const int c) {
    uint64_t mask = 0;
    for (int r = 0; r < 8; ++r)
        mask |= coord_to_bit(r, c);
    return mask;
}

// Main diagonal: r - c = k, where k in [-7...+7] we store from 0..14 by k+7
constexpr uint64_t main_diag_mask(const int idx) {
    const int k = idx - 7;
    uint64_t mask = 0;
    for (int r = 0; r < 8; ++r) {
        if (const int c = r - k; c >= 0 && c < 8)
            mask |= coord_to_bit(r, c);
    }
    return mask;
}

// Anti-diagonal: r + c = k, where k in [0..14]
constexpr uint64_t anti_diag_mask(const int k) {
    uint64_t mask = 0;
    for (int r = 0; r < 8; ++r) {
        if (const int c = k - r; c >= 0 && c < 8)
            mask |= coord_to_bit(r, c);
    }
    return mask;
}

// Precompute masks
constexpr std::array<uint64_t, 8> ROW_MASKS = [] {
    std::array<uint64_t, 8> arr{};
    for (int r = 0; r < 8; ++r)
        arr[r] = row_mask(r);
    return arr;
}();

constexpr std::array<uint64_t, 8> COL_MASKS = [] {
    std::array<uint64_t, 8> arr{};
    for (int c = 0; c < 8; ++c)
        arr[c] = col_mask(c);
    return arr;
}();

constexpr std::array<uint64_t, 15> MAIN_DIAG_MASKS = [] {
    std::array<uint64_t, 15> arr{};
    for (int i = 0; i < 15; ++i)
        arr[i] = main_diag_mask(i);
    return arr;
}();

constexpr std::array<uint64_t, 15> ANTI_DIAG_MASKS = [] {
    std::array<uint64_t, 15> arr{};
    for (int i = 0; i < 15; ++i)
        arr[i] = anti_diag_mask(i);
    return arr;
}();

#endif
