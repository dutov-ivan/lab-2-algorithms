#ifndef ASTAR_H
#define ASTAR_H

#include "search.h"

#include <functional>
#include <vector>
#include <queue>

#include "heuristic.h"


struct AStarNode {
    unsigned int f; // Estimated total cost (g + h)
    unsigned int g; // Cost from start to current node
    Board bitboard; // Unique representation of board with queens

    AStarNode(const Board board, const unsigned int g, const std::unique_ptr<Heuristic> &h) {
        this->bitboard = board;
        this->g = g;
        this->f = g + h->calculate(board);
    }
};

typedef std::priority_queue<AStarNode, std::vector<AStarNode>, std::function<bool
    (const AStarNode &, const AStarNode &)> > AStarPriorityQueue;


class AStarSearch final : public Search {
public:
    explicit AStarSearch() : Search("AStar Search") {
    }

    SearchResult search(Board start, const std::unique_ptr<Heuristic> &h) override;

private:
    static void add_next_states(const AStarNode &initial, AStarPriorityQueue &pq, const std::unique_ptr<Heuristic> &h);
};

#endif // ASTAR_H
