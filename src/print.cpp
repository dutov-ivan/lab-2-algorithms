#include "../include/print.h"

#include <iostream>
void print_bitboard(std::uint64_t bitboard)
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (bitboard & (1ULL << (col * 8 + row))) {
                std::cout << " Q ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << std::endl;
    }
}