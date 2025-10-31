#ifndef LOCAL_H
#define LOCAL_H
#include <memory>
#include <random>
#include "heuristic.h"

#include "search.h"

class AnnealingSearch final : public HeuristicSearch {
public:
    explicit AnnealingSearch(std::mt19937 &gen, const std::shared_ptr<Heuristic> &h);

    SearchResult search(Board start) override;

private:
    static double T(int t);

    static bool is_valid_board(const Board &start);

    Board next_state(const Board &current, SearchStats &stats);

    std::mt19937 &gen_;
    std::uniform_int_distribution<std::uint8_t> col_distribution_;
    std::uniform_int_distribution<std::uint8_t> col_distribution_except_one_;
};

struct BacktrackNode {
    Board board;
    std::uint8_t emptyCols;

    explicit BacktrackNode(Board board);

    explicit BacktrackNode(const Board board, const std::uint8_t emptyCols) : board(board), emptyCols(emptyCols) {
    }
};

class BacktrackSearch final : public Search {
public:
    explicit BacktrackSearch() : Search("Backtracking Search") {
    }

    SearchResult search(Board start) override;
};

class AnnealingThenBacktrack final : public HeuristicSearch {
public:
    explicit AnnealingThenBacktrack(std::mt19937 &gen, const std::shared_ptr<Heuristic> &h);

    SearchResult search(Board start) override;

private:
    static Board remove_conflicts(Board board);

    std::mt19937 &gen_;
    std::unique_ptr<AnnealingSearch> annealing_;
    std::unique_ptr<BacktrackSearch> backtracking_;
};

#endif // LOCAL_H
