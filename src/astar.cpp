#include "../include/astar.h"

#include <queue>


AStarSearch::AStarSearch(HeuristicFunction h): heuristic(h) {

}

SearchResult AStarSearch::search(Board start) {
    SearchStats stats;
    auto initialState = SearchNode(start, 0);
    auto cmp = [this](const SearchNode& a, const SearchNode& b) {
        return (a.g + heuristic(a.bitboard)) > (b.g + heuristic(b.bitboard));
    };
    std::priority_queue<SearchNode, std::vector<SearchNode>, decltype(cmp)> pq(cmp);

    pq.push(initialState);
    while (!pq.empty()) {
        SearchNode current = pq.top();
        pq.pop();

        if (heuristic(current.bitboard) == 0) {
            return ;
        }

        int queenCol = current.g % 8; // Determine which queen to move based on g value
        std::vector<SearchNode> nextStates = get_next_states(current, queenCol);
        for (SearchNode& nextState : nextStates) {
            pq.push(nextState);
        }

        stats.iterations++;
    }

    return 0;
}

std::vector<SearchNode> get_next_states(SearchNode& state, int queenCol)
{
    std::vector<SearchNode> newBitboards;

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

        newBitboards.push_back(SearchNode(newBitboard, state.g + 1));
    }
    return newBitboards;
}

