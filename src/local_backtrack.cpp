#include "../include/local_backtrack.h"
#include <stack>
#include <iostream>

#include "generate.h"
#include "print.h"

AnnealingSearch::AnnealingSearch(std::mt19937 &gen) : Search("simulated annealing search"), gen_(gen),
                                                      col_distribution_(0, 7),
                                                      col_distribution_except_one_(0, 7 - 1) {
}

SearchResult AnnealingSearch::search(const Board start, const std::unique_ptr<Heuristic> &h) {
    if (!is_valid_board(start))
        return SearchResult{SearchStats{}, start, false};

    int t = 1;
    Board current = start;
    SearchStats stats;
    stats.nodesInMemory = 2;

    while (T(t) != 0) {
        if (h->calculate(current) == 0) {
            return SearchResult{stats, current, current.count_queens() == 8}; // Found a solution
        }


        const Board randomState = next_state(current, stats);

        if (const double deltaE = h->calculate(randomState) - h->calculate(current); deltaE > 0) {
            current = randomState;
        } else {
            if (const double acceptanceProb = exp(deltaE / T(t));
                static_cast<double>(gen_()) / static_cast<double>(std::mt19937::max()) < acceptanceProb) {
                current = randomState;
            }
        }

        t++;
        stats.iterations++;
    }
    return SearchResult{stats, current, false}; // No solution found
}


double AnnealingSearch::T(const int t) {
    return 1000.0 - 10.0 * t;
}

bool AnnealingSearch::is_valid_board(const Board &start) {
    std::uint8_t columnMask = 0;
    int queenCount = 0;

    for (const auto &sq: start) {
        const int col = sq % 8;
        const uint8_t bit = 1u << col;

        // Duplicate column found
        if (columnMask & bit) {
            return false;
        }

        columnMask |= bit;
        queenCount++;
    }

    // All 8 columns used exactly once, and exactly 8 queens placed
    return columnMask == 0xFF && queenCount == 8;
}


Board AnnealingSearch::next_state(const Board &current, SearchStats &stats) {
    const std::uint8_t queenCol = col_distribution_(gen_);

    int queenRow = -1;
    for (std::uint8_t row = 0; row < 8; ++row) {
        if (current.has_queen_at(queen_position(queenCol, row))) {
            queenRow = row;
            break;
        }
    }

    std::uint8_t generated_row = col_distribution_except_one_(gen_);
    if (generated_row >= queenRow) {
        generated_row += 1; // Skip the current row
    }

    Board randomState = current;
    randomState.clear_queen(queen_position(queenCol, queenRow))
            ->set_queen(queen_position(queenCol, generated_row));
    stats.nodesGenerated++;
    return randomState;
}

BacktrackNode::BacktrackNode(const Board board) {
    uint8_t empty_cols = 0xFF; // all 8 bits = 1, assume all columns empty

    for (const auto &q: board) {
        const std::uint8_t col = q % 8;
        empty_cols &= ~(1u << col);
    }

    this->emptyCols = empty_cols;
    this->board = board;
}

SearchResult BacktrackSearch::search(const Board start, const std::unique_ptr<Heuristic> &h) {
    SearchStats stats;
    std::stack<BacktrackNode> st;
    st.emplace(start);

    while (!st.empty()) {
        BacktrackNode current = st.top();
        st.pop();

        if (current.emptyCols == 0 && current.board.count_queens() == 8) {
            return SearchResult(stats, current.board, true);
        }

        const uint8_t col = __builtin_ctzll(current.emptyCols);
        bool expanded = false;

        for (int r = 0; r < 8; ++r) {
            const int pos = r * 8 + col;

            if (current.board.queens_attacked_from(pos).get() != 0) continue;

            Board newBoard = current.board;
            newBoard.set_queen(pos);

            uint64_t newEmpty = current.emptyCols & ~(1ULL << col);
            st.emplace(newBoard, newEmpty);
            stats.nodesGenerated++;
            expanded = true;
        }

        if (!expanded) stats.deadEnds++;
        stats.nodesInMemory = std::max(stats.nodesInMemory, st.size());
        stats.iterations++;
    }

    return SearchResult(stats, Board(0), false);
}


AnnealingThenBacktrack::AnnealingThenBacktrack(std::mt19937 &gen) : Search(
                                                                        "Annealing then Backtracking Search with retries"),

                                                                    gen_(gen) {
    annealing_ = std::make_unique<AnnealingSearch>(gen);
    backtracking_ = std::make_unique<BacktrackSearch>();
}

SearchResult AnnealingThenBacktrack::search(Board start, const std::unique_ptr<Heuristic> &h) {
    SearchResult globalSearchResult;

    do {
        const auto [localStats, localBoard, localSolved] = annealing_->search(start, h);
        depict_state(localBoard, h);
        globalSearchResult.stats += localStats;
        if (localSolved) {
            break;
        }

        const Board board_without_conflicts = remove_conflicts(localBoard);
        depict_state(board_without_conflicts, h);

        const auto [stats, solution, solved] = backtracking_->search(board_without_conflicts, h);
        globalSearchResult.stats += stats;

        if (!solved) {
            std::cout << "Encountered a sad ending...\n" << std::endl;
            start = generate_initial_bitboard(gen_);
        } else {
            globalSearchResult.solution = solution;
            globalSearchResult.solved = true;
        }
    } while (!globalSearchResult.solved);

    std::cout << "Encountered a happy ending...\n" << std::endl;
    return globalSearchResult;
}

Board AnnealingThenBacktrack::remove_conflicts(Board board) {
    bool hasConflicts = true;
    while (hasConflicts) {
        int maxConflictsPos = -1;
        int maxConflicts = 0;

        for (const auto &sq: board) {
            Board attackedQueens = board.queens_attacked_from(sq);

            if (const int conflicts = attackedQueens.count_queens(); conflicts > maxConflicts) {
                maxConflicts = conflicts;
                maxConflictsPos = sq;
            }
        }

        if (maxConflicts == 0)
            hasConflicts = false;
        else board.clear_queen(maxConflictsPos);
    }

    return board;
}

