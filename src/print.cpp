#include "../include/print.h"

#include <iostream>
#include <memory>

#include "common.h"
#include "heuristic.h"

void print_bitboard(const Board &board) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.has_queen_at(queen_position(col, row))) {
                std::cout << " Q ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << std::endl;
    }
}

void depict_state(const Board &board, const std::unique_ptr<Heuristic> &h) {
    print_bitboard(board);
    const double h_value = h->calculate(board);
    std::cout << "Heuristic value: " << h_value << std::endl;
}
