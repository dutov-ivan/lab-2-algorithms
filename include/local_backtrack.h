#ifndef LOCAL_H
#define LOCAL_H
#include <cstdint>
#include <memory>
#include <vector>
#include <random>

#include "search.h"

class AnnealingSearch final : public Search {
public:
    explicit AnnealingSearch(std::mt19937& gen, HeuristicFunction h);

    static bool is_valid_board(const Board &start);

    SearchResult search(Board start) override;

    static double T(int t);
private:
    Board next_state(const Board &current, SearchStats &stats);
    std::mt19937& gen_;
    HeuristicFunction h_;
    std::uniform_int_distribution<std::uint8_t> col_distribution_;
    std::uniform_int_distribution<std::uint8_t> col_distribution_except_one_;
};

struct SearchNode {
    Board board;
    std::uint8_t emptyCols;

    SearchNode(Board board);
};

class BacktrackSearch final : public Search {
public:
    explicit BacktrackSearch(HeuristicFunction h);
    SearchResult search(Board start) override;
private:
    HeuristicFunction h_;
};

class AnnealingThenBacktrack final : public Search {
public:
    explicit AnnealingThenBacktrack(std::mt19937& gen, HeuristicFunction h);
    SearchResult search(Board start) override;
private:
    static Board remove_conflicts(Board board);
    HeuristicFunction h_;
    std::unique_ptr<AnnealingSearch> annealing_;
    std::unique_ptr<BacktrackSearch> backtracking_;
};

#endif // LOCAL_H