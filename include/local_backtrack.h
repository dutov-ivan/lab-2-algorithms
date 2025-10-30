#ifndef LOCAL_H
#define LOCAL_H
#include <cstdint>
#include <memory>
#include <vector>
#include <random>
#include "heuristic.h"

#include "search.h"

class AnnealingSearch final : public Search {
public:
    explicit AnnealingSearch(std::mt19937 &gen);

    SearchResult search(Board start, const std::unique_ptr<Heuristic> &h) override;

private:
    static double T(int t);

    static bool is_valid_board(const Board &start);

    Board next_state(const Board &current, SearchStats &stats);

    std::mt19937 &gen_;
    std::uniform_int_distribution<std::uint8_t> col_distribution_;
    std::uniform_int_distribution<std::uint8_t> col_distribution_except_one_;
};

struct SearchNode {
    Board board;
    std::uint8_t emptyCols;

    explicit SearchNode(Board board);

    explicit SearchNode(const SearchNode &prev, std::uint8_t col, std::uint8_t row);
};

class BacktrackSearch final : public Search {
public:
    explicit BacktrackSearch() : Search("Backtracking Search") {
    }

    SearchResult search(Board start, const std::unique_ptr<Heuristic> &h) override;
};

class AnnealingThenBacktrack final : public Search {
public:
    explicit AnnealingThenBacktrack(std::mt19937 &gen);

    SearchResult search(Board start, const std::unique_ptr<Heuristic> &h) override;

private:
    static Board remove_conflicts(Board board);

    std::mt19937 &gen_;
    std::unique_ptr<AnnealingSearch> annealing_;
    std::unique_ptr<BacktrackSearch> backtracking_;
};

#endif // LOCAL_H
