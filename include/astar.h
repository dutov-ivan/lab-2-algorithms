#ifndef ASTAR_H
#define ASTAR_H

#include "search.h"

#include <cstdint>
#include <vector>

struct SearchNode {
    SearchNode(uint64_t b, int g) : bitboard(b), g(g) {}
    int g; // Cost from start to current node
    std::uint64_t bitboard; // Unique representation of board with queens
};

class AStarSearch final : public Search {
public:
    explicit AStarSearch(HeuristicFunction h);

    SearchResult search(Board start) override;
private:
    int (*heuristic)(Board board);
};

uint64_t astar(std::uint64_t start, int (*h)(std::uint64_t));

std::vector<SearchNode> get_next_states(SearchNode& state, int queenPos);

#endif // ASTAR_H