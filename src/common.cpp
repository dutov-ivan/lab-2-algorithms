//
// Created by dutov on 10/22/2025.
//
#include "../include/common.h"

#include "constants.h"

std::uint64_t Board::get() {
    return bitboard;
}

Board *Board::clear_queen(std::uint8_t position) {
    bitboard &= ~(1ULL << position);
    return this;
}

bool Board::has_queen_at(std::uint8_t position) const {
    return (bitboard & (1ULL << position)) != 0;
}

Board *Board::set_queen(std::uint8_t position) {
    bitboard |= (1ULL << position);
    return this;
}

Board Board::queens_attacked_from(const std::uint8_t position) const {
    return Board(QUEEN_ATTACKS[position] & bitboard);
}

std::uint8_t Board::count_queens() const {
    return __builtin_popcountll(bitboard);
}

bool Board::is_empty() const {
    return bitboard == 0;
}

std::uint8_t queen_position(const std::uint8_t col, const std::uint8_t row) {
    return col * 8 + row;
}



