#include "../include/astar.h"

#include <queue>


AStarSearch::AStarSearch(HeuristicFunction h) : heuristic(h) {
}

SearchResult AStarSearch::search(Board start) {
    SearchStats stats;
    auto initialState = AstarNode{0, start};
    auto cmp = [this](const AstarNode &a, const AstarNode &b) {
        return (a.g + heuristic(a.bitboard)) > (b.g + heuristic(b.bitboard));
    };
    std::priority_queue<AstarNode, std::vector<AstarNode>, decltype(cmp)> pq(cmp);

    pq.push(initialState);
    while (!pq.empty()) {
        if (pq.size() > stats.nodesInMemory) {
            stats.nodesInMemory = pq.size();
        }
        AstarNode current = pq.top();
        pq.pop();

        if (heuristic(current.bitboard) == 0) {
            return SearchResult{stats, current.bitboard, true};
        }

        const std::uint8_t queenCol = current.g % 8; // Determine which queen to move based on g value
        std::vector<AstarNode> nextStates = get_next_states(current, queenCol);
        stats.nodesGenerated += nextStates.size();
        for (AstarNode &nextState: nextStates) {
            pq.push(nextState);
        }

        stats.iterations++;
    }

    return SearchResult{stats, Board(0), false};
}


std::vector<AstarNode> AStarSearch::get_next_states(const AstarNode &initial, const std::uint8_t queenCol) {
    std::vector<AstarNode> newBitboards;

    for (int row = 0; row < 8; ++row) {
        const uint8_t new_position = queen_position(queenCol, row);
        if (initial.bitboard.has_queen_at(new_position)) {
            continue; // Skip if there's a queen already in this position
        }

        Board newBitboard = initial.bitboard;

        // Clear the current queen position in the specified column
        for (int r = 0; r < 8; ++r) {
            newBitboard.clear_queen(queen_position(queenCol, r));
        }

        // Set the new queen position in the specified column
        newBitboard.set_queen(new_position);

        newBitboards.emplace_back(AstarNode{initial.g + 1, newBitboard});
    }
    return newBitboards;
}


