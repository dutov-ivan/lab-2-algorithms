#include <iostream>
#include "../include/generate.h"
#include "../include/astar.h"
#include "../include/print.h"
#include "../include/heuristic.h"
#include "../include/local.h"

void depict_state(uint64_t board) {
    print_bitboard(board);
    int h_value = count_attacking_pairs(board);
    std::cout << "Heuristic value: " << h_value << std::endl;
}

void local_and_backtrack_workflow(std::mt19937& gen) {
    bool encountered_sad_ending = true;

    do {
        std::uint64_t initialState = generate_initial_bitboard(gen);
        depict_state(initialState);

        uint64_t localSearchResult = local_search(initialState, count_attacking_pairs, gen);
        depict_state(localSearchResult);

        std::uint64_t board_without_conflicts = remove_conflicts(localSearchResult);
        depict_state(board_without_conflicts);

        BacktrackResult backtrack_result = backtrack(board_without_conflicts);
        encountered_sad_ending = !backtrack_result.found;
        if (encountered_sad_ending) {
            std::cout << "Encountered a sad ending...\n" << std::endl;
        } else {
            std::cout << "Encountered a happy ending...\n" << std::endl;
        }

        depict_state(backtrack_result.board);
    } while(encountered_sad_ending);
}

void backtrack_workflow() {
    std::uint64_t initial = 0;
    depict_state(initial);

    BacktrackResult result = backtrack(initial);
    depict_state(result.board);
}

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());
    
    local_and_backtrack_workflow(gen);
    

    return 0;
}
