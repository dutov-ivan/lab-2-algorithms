#include <iostream>
#include "../include/generate.h"
#include "../include/astar.h"
#include "../include/print.h"
#include "../include/heuristic.h"
#include "../include/local_backtrack.h"

int main(int argc, char const *argv[]) {
    std::random_device rd;
    std::mt19937 gen(rd());

    const Board initialBoard = generate_initial_bitboard(gen);
    depict_state(initialBoard, count_attacking_pairs);

    AnnealingThenBacktrack searcher(gen, count_attacking_pairs);
    SearchResult result = searcher.search(initialBoard);
    depict_state(result.solution, count_attacking_pairs);

    return 0;
}
