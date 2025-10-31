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
    Board board; // Unique representation of board with queens

    AStarNode(const Board board, const unsigned int g, const std::shared_ptr<Heuristic> &h) {
        this->board = board;
        this->g = g;
        this->f = g + h->calculate(board);
    }
};

typedef std::priority_queue<AStarNode, std::vector<AStarNode>, std::function<bool
    (const AStarNode &, const AStarNode &)> > AStarPriorityQueue;


class AStarSearch final : public HeuristicSearch {
public:
    explicit AStarSearch(const std::shared_ptr<Heuristic> &h) : HeuristicSearch("AStar Search", h) {
    }

    SearchResult search(Board start) override;

private:
    static void add_next_states(const AStarNode &initial, AStarPriorityQueue &pq, const std::shared_ptr<Heuristic> &h);
};

#endif // ASTAR_H
