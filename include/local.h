#ifndef LOCAL_H
#define LOCAL_H
#include <cstdint>
#include <vector>
#include <random>

struct BacktrackResult {
    std::uint64_t board;
    bool found;

    BacktrackResult(std::uint64_t b, bool f) : board(b), found(f) {}
};

int T(int t);
uint64_t local_search(std::uint64_t start, int (*h)(std::uint64_t), std::mt19937& gen);
uint64_t remove_conflicts(std::uint64_t board);
BacktrackResult backtrack(std::uint64_t start);
std::vector<uint64_t> get_next_local_states(std::uint64_t bitboard, int queenCol);

#endif // LOCAL_H