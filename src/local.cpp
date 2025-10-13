#include "../include/local.h"
#include "../include/constants.h"
#include "local.h"
#include <stack>

struct BacktrackState {
    std::uint64_t board;
    std::uint8_t emptyCols;

    BacktrackState(std::uint64_t board);
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


// We use empty columns for backtracking because this way we would have clarly defined invalid state (when the queens hit each other)
uint64_t backtrack(std::uint64_t start)
{
    BacktrackState initial(start);
    std::stack<BacktrackState> st;
    st.push(initial);

    while(!st.empty()){
        BacktrackState next = st.top();
        st.pop();
        std::uint8_t emptyCols = next.emptyCols;
        if (emptyCols == 0) {
            return next.board;
        }

        while(emptyCols != 0) {
            int col = __builtin_ctzll(emptyCols);
            for (int r = 0; r < 8; r++){
                const int pos = r * 8 + col;
                std::uint64_t overlap = QUEEN_ATTACKS[pos] & (next.board);
                if (overlap != 0) {
                    continue;
                }
                std::uint64_t newBoard = next.board + (1ULL << pos);
                BacktrackState newState = BacktrackState(newBoard);
                st.push(newState);
            }
            emptyCols &= emptyCols - 1;
        }
    }
    return 0;
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

BacktrackState::BacktrackState(uint64_t board)
{
    uint8_t emptyCols = 0xFF; // all 8 bits = 1, assume all columns empty
    uint64_t copyBoard = board;

    while (copyBoard) {
        int col = __builtin_ctzll(copyBoard) % 8;  // column index 0..7
        emptyCols &= ~(1u << col);                 // mark column as non-empty
        copyBoard &= copyBoard - 1;                // clear lowest set bit
    }

    this->emptyCols = emptyCols; // assuming you store it in the struct/class
}
