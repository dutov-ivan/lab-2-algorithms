#include "../include/astar.h"

#include <queue>
#include <cstdint>
#include <array>
#include <iostream>

constexpr std::array<uint64_t, 64> generate_queen_attacks() {
    std::array<uint64_t, 64> attacks{};

    for (int sq = 0; sq < 64; ++sq) {
        int r = sq / 8;
        int f = sq % 8;
        uint64_t mask = 0;

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

constexpr auto QueenAttacks = generate_queen_attacks();

uint64_t astar(std::uint64_t start, int (*h)(std::uint64_t))
{
    State initialState = State(start, 0);
    auto cmp = [h](const State& a, const State& b) {
        return (a.g + h(a.bitboard)) > (b.g + h(b.bitboard));
    };
    std::priority_queue<State, std::vector<State>, decltype(cmp)> pq(cmp);

    pq.push(initialState);
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (h(current.bitboard) == 0) {
            return current.bitboard;
        }

        int queenCol = current.g % 8; // Determine which queen to move based on g value
        std::vector<State> nextStates = get_next_states(current, queenCol);
        for (State& nextState : nextStates) {
            pq.push(nextState);
        }
    }

    return 0;
}

std::vector<State> get_next_states(State& state, int queenCol)
{
    std::vector<State> newBitboards;

    for (int row = 0; row < 8; ++row) {
        std::uint64_t newBitboard = state.bitboard;
        if (newBitboard & (1ULL << (queenCol * 8 + row))) {
            continue; // Skip if there's a queen already in this position
        }

        // Clear the current queen position in the specified column
        for (int r = 0; r < 8; ++r) {
            newBitboard &= ~(1ULL << (queenCol * 8 + r));
        }

        // Set the new queen position in the specified column
        newBitboard |= (1ULL << (queenCol * 8 + row));

        newBitboards.push_back(State(newBitboard, state.g + 1));
    }
    return newBitboards;
}

int count_attacking_pairs(uint64_t board) {
    int total_attacks = 0;

    while (board != 0) {
        // 1. Find the index of the least significant queen (0-63)
        int queen_index = __builtin_ctzll(board);

        // 2. Get the pre-computed attack mask for this queen
        uint64_t attack_mask = QueenAttacks[queen_index];

        // 3. "Remove" this queen from the board for the next iteration's check
        board &= board - 1;

        // 4. Count how many of the REMAINING queens are on the attack mask
        // __builtin_popcountll is a GCC/Clang intrinsic for "Population Count"
        total_attacks += __builtin_popcountll(board & attack_mask);
    }

    return total_attacks;
}
