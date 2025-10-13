#ifndef ASTAR_H
#define ASTAR_H

#include <cstdint>
#include <vector>

struct State {
    State(uint64_t b, int g) : bitboard(b), g(g) {}
    int g; // Cost from start to current node
    std::uint64_t bitboard; // Unique representation of board with queens
};

uint64_t astar(std::uint64_t start, int (*h)(std::uint64_t));

std::vector<State> get_next_states(State& state, int queenPos);

int count_attacking_pairs(std::uint64_t state);

#endif // ASTAR_H