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
    explicit AStarSearch();

    SearchResult search(Board start, HeuristicFunction h) override;

private:
    static std::vector<AstarNode> get_next_states(const AstarNode &initial, std::uint8_t queenCol);
};

#endif // ASTAR_H
