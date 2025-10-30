//
// Created by dutov on 10/22/2025.
//
#include "../include/common.h"

#include <stdexcept>

#include "masks.h"

std::uint64_t Board::get() const {
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

int Board::queen_row(std::uint8_t col) const {
    for (const auto &sq: *this) {
        if (sq % 8 == col) {
            return sq / 8;
        }
    }
    return -1;
}

std::uint8_t queen_position(const std::uint8_t col, const std::uint8_t row) {
    return row * 8 + col;
}



