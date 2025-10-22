#include "../include/local.h"
#include "../include/constants.h"
#include "local.h"
#include <stack>

struct SearchNode {
    std::uint64_t board;
    std::uint8_t emptyCols;

    SearchNode(std::uint64_t board);
};

int T(int t)
{
    return 1000 - 10 * t;
}

uint64_t local_search(std::uint64_t start, int (*h)(std::uint64_t), std::mt19937& gen)
{
    int t = 1;
    std::uint64_t current = start;
    while (T(t) != 0) {
        if (h(current) == 0) {
            return current; // Found a solution
        }

        std::vector<uint64_t> nextStates = get_next_local_states(current, t % 8);
        uint64_t randomState = nextStates[gen() % nextStates.size()];
        int deltaE = h(randomState) - h(current);
        if (deltaE > 0) {
            current = randomState;
        } else {
            double acceptanceProb = exp(deltaE / T(t));
            if ((gen() / static_cast<double>(gen.max())) < acceptanceProb) {
                current = randomState;
            }
        }

        t++;
    }
    return current;
}

uint64_t remove_conflicts(uint64_t board) {
    while (true) {

        int maxConflictsPos = -1;
        int maxConflicts = 0;

        uint64_t b = board;
        while (b) {
            int pos = __builtin_ctzll(b);
            uint64_t attacks = QUEEN_ATTACKS[pos] & board;
            attacks &= ~(1ULL << pos); // exclude itself
            int conflicts = __builtin_popcountll(attacks);

            if (conflicts > maxConflicts) {
                maxConflicts = conflicts;
                maxConflictsPos = pos;
            }
            b &= b - 1;
        }

        if (maxConflicts == 0)
            break; // no conflicts left

        // remove the queen causing most conflicts
        board &= ~(1ULL << maxConflictsPos);
    }

    return board;
}

bool is_there_empty_place(std::uint64_t bitboard) {
    std::uint64_t attackedSquares = 0;
    while (bitboard) {
        int pos = __builtin_ctzll(bitboard);
        uint64_t attacks = QUEEN_ATTACKS[pos] & bitboard;
        attacks &= ~(1ULL << pos);
        attackedSquares |= attacks;
        bitboard &= bitboard - 1; 
    }
    return attackedSquares != ~0ULL;
}




BacktrackResult backtrack(std::uint64_t start)
{

    if (!is_there_empty_place(start)) {
        return BacktrackResult(start, false); // No empty places, return the current board
    }

    SearchNode initial(start);
    std::stack<SearchNode> st;
    st.push(initial);

    while(!st.empty()){
        SearchNode current = st.top();
        st.pop();
        
        std::uint8_t emptyCols = current.emptyCols;

        // Check for solution: All columns are filled
        if (emptyCols == 0) {
            // We also need to ensure it's 8 queens, in case the 'start' board was partial
            if (__builtin_popcountll(current.board) == 8) {
                return BacktrackResult(current.board, true); // Found a solution
            }
        }

        int col = __builtin_ctzll(emptyCols);

        bool newStateCreated = false;
        // Now, iterate through all rows *for that single column*
        for (int r = 0; r < 8; r++){
            const int pos = r * 8 + col;
            std::uint64_t overlap = QUEEN_ATTACKS[pos] & (current.board);
            
            // If placing a queen here conflicts, skip this row
            if (overlap != 0) {
                continue;
            }

            // This position (r, col) is valid. Create the new child state.
            // Using | (bitwise OR) is more idiomatic than + for setting a bit.
            std::uint64_t newBoard = current.board | (1ULL << pos); 
            
            SearchNode newState = SearchNode(newBoard);
            st.push(newState); // Push the new state to be explored
            newStateCreated = true;
        }

        if (!newStateCreated) {
            // No valid positions found in this column, so this path is a dead end.
            continue;
        }
        
        // The 'while(emptyCols != 0)' loop is gone.
    }
    return BacktrackResult(0, false); // No solution found
}

std::vector<uint64_t> get_next_local_states(std::uint64_t bitboard, int queenCol)
{
    std::vector<uint64_t> newBitboards;

    for (int row = 0; row < 8; ++row) {
        std::uint64_t newBitboard = bitboard;
        if (newBitboard & (1ULL << (queenCol * 8 + row))) {
            continue; // Skip if there's a queen already in this position
        }

        // Clear the current queen position in the specified column
        for (int r = 0; r < 8; ++r) {
            newBitboard &= ~(1ULL << (queenCol * 8 + r));
        }

        // Set the new queen position in the specified column
        newBitboard |= (1ULL << (queenCol * 8 + row));

        newBitboards.push_back(newBitboard);
    }
    return newBitboards;
}

SearchNode::SearchNode(uint64_t board)
{
    uint8_t emptyCols = 0xFF; // all 8 bits = 1, assume all columns empty
    uint64_t copyBoard = board;

    while (copyBoard) {
        int col = __builtin_ctzll(copyBoard) % 8;  // column index 0..7
        emptyCols &= ~(1u << col);                 // mark column as non-empty
        copyBoard &= copyBoard - 1;                // clear lowest set bit
    }

    this->emptyCols = emptyCols; // assuming you store it in the struct/class
    this->board = board;
}
