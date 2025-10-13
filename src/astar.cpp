#include "../include/astar.h"

#include <queue>


uint64_t astar(std::uint64_t start, int (*h)(std::uint64_t))
{
    BacktrackState initialState = BacktrackState(start, 0);
    auto cmp = [h](const BacktrackState& a, const BacktrackState& b) {
        return (a.g + h(a.bitboard)) > (b.g + h(b.bitboard));
    };
    std::priority_queue<BacktrackState, std::vector<BacktrackState>, decltype(cmp)> pq(cmp);

    pq.push(initialState);
    while (!pq.empty()) {
        BacktrackState current = pq.top();
        pq.pop();

        if (h(current.bitboard) == 0) {
            return current.bitboard;
        }

        int queenCol = current.g % 8; // Determine which queen to move based on g value
        std::vector<BacktrackState> nextStates = get_next_states(current, queenCol);
        for (BacktrackState& nextState : nextStates) {
            pq.push(nextState);
        }
    }

    return 0;
}

std::vector<BacktrackState> get_next_states(BacktrackState& state, int queenCol)
{
    std::vector<BacktrackState> newBitboards;

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

        newBitboards.push_back(BacktrackState(newBitboard, state.g + 1));
    }
    return newBitboards;
}

