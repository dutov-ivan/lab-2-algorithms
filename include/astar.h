#ifndef ASTAR_H
#define ASTAR_H

#include <cstdint>
#include <vector>

struct BacktrackState {
    BacktrackState(uint64_t b, int g) : bitboard(b), g(g) {}
    int g; // Cost from start to current node
    std::uint64_t bitboard; // Unique representation of board with queens
};

uint64_t astar(std::uint64_t start, int (*h)(std::uint64_t));

std::vector<BacktrackState> get_next_states(BacktrackState& state, int queenPos);

#endif // ASTAR_H