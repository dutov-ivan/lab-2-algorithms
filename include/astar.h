#ifndef ASTAR_H
#define ASTAR_H

#include "search.h"

#include <cstdint>
#include <vector>

struct AstarNode {
    unsigned int g; // Cost from start to current node
    Board bitboard; // Unique representation of board with queens
};

class AStarSearch final : public Search {
public:
    explicit AStarSearch(HeuristicFunction h);

    SearchResult search(Board start) override;
private:
    static std::vector<AstarNode> get_next_states(const AstarNode& initial, std::uint8_t queenCol);
    HeuristicFunction heuristic;
};

#endif // ASTAR_H