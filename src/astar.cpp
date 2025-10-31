#include "../include/astar.h"

#include <unordered_set>


SearchResult AStarSearch::search(const Board start) {
    SearchStats stats;
    std::unordered_set<uint64_t> visited;
    const auto initialState = AStarNode{start, 0, h()};
    auto cmp = [](const AStarNode &a, const AStarNode &b) {
        return a.f > b.f;
    };
    AStarPriorityQueue pq(cmp);

    pq.push(initialState);
    while (!pq.empty()) {
        if (pq.size() > stats.nodesInMemory) {
            stats.nodesInMemory = pq.size();
        }
        AStarNode current = pq.top();
        pq.pop();

        if (visited.contains(current.board.get())) continue;
        visited.insert(current.board.get());

        if (h()->calculate(current.board) == 0) {
            return SearchResult{stats, current.board, true};
        }

        const size_t before = pq.size();
        add_next_states(current, pq, h());
        stats.nodesGenerated += pq.size() - before;

        stats.iterations++;
    }

    return SearchResult{stats, Board(0), false};
}


void AStarSearch::add_next_states(const AStarNode &initial, AStarPriorityQueue &pq,
                                  const std::shared_ptr<Heuristic> &h) {
    for (int col = 0; col < 8; ++col) {
        const uint8_t currentRow = initial.board.queen_row(col);
        for (int row = 0; row < 8; ++row) {
            if (row == currentRow) {
                continue; // Skip if there's a queen already in this position
            }

            Board newBitboard = initial.board;

            newBitboard.clear_queen(queen_position(col, currentRow));
            newBitboard.set_queen(queen_position(col, row));

            pq.emplace(newBitboard, initial.g + 1, h);
        }
    }
}


