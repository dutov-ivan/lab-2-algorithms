#include <iostream>
#include "../include/generate.h"
#include "../include/astar.h"
#include "../include/print.h"

#include <random>

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uint64_t initialState = generate_initial_bitboard(gen);
    print_bitboard(initialState);
    int h_value = count_attacking_pairs(initialState);
    std::cout << "Heuristic value: " << h_value << std::endl;

    uint64_t result = astar(initialState, count_attacking_pairs);
    print_bitboard(result);
    if (result != 0) {
        std::cout << "Solution found!" << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}
